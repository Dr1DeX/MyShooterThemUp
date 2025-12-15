// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STURifleWeaponActor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ASTURifleWeaponActor::ASTURifleWeaponActor()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeaponActor::StartFire()
{
    InitFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeaponActor::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeaponActor::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetFXActive(false);
}

void ASTURifleWeaponActor::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector CameraStart, CameraEnd;
    if (!GetTraceData(CameraStart, CameraEnd))
    {
        StopFire();
        return;
    }

    FShotTraceResult Shot;
    if (!MakeHit(World, CameraStart, CameraEnd, Shot))
        return;

    FVector TraceFXEnd = CameraEnd;
    
    //const FVector EndPoint = Shot.bHit ? Shot.Hit.ImpactPoint : Shot.MuzzleEnd;
    //DrawDebugLine(World, Shot.MuzzleStart, EndPoint, FColor::Red, false, 3.0f, 0, 3.0f);

    if (Shot.bHit)
    {
        TraceFXEnd = Shot.Hit.ImpactPoint;
        //DrawDebugSphere(World, Shot.Hit.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        MakeDamage(Shot.Hit, Shot.DirFromMuzzle);
        WeaponFXComponent->PlayImpactFX(Shot.Hit);
    }
    SpawnTraceFX(GetMuzzleTM().GetLocation(), TraceFXEnd);
    DecreaseAmmo();
}

bool ASTURifleWeaponActor::GetTraceData(FVector& CameraTraceStart, FVector& CameraTraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    CameraTraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    CameraTraceEnd = CameraTraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeaponActor::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}

void ASTURifleWeaponActor::MakeDamage(const FHitResult& ShotHit, const FVector& DirFromMuzzle)
{
    float ActualDamage = BaseDamage;

    if (ShotHit.BoneName != NAME_None)
    {
        const FString Bone = ShotHit.BoneName.ToString().ToLower();
        if (Bone.Contains(TEXT("head")))
        {
            ActualDamage *= HeadshotMultiplier;
        }
    }

    AController* InstigatorController = nullptr;
    if (const auto* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        InstigatorController = OwnerChar->GetController();
    }

    UGameplayStatics::ApplyPointDamage(ShotHit.GetActor(), // Target
        ActualDamage,                                      // Damage
        DirFromMuzzle,                                     // Shot direction
        ShotHit,                                           // Detail Hit
        InstigatorController,                              // Instigator
        this,                                              // Causer (weapon)
        DamageType                                         // Damage Type
    );
}

void ASTURifleWeaponActor::InitFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }

    if(!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
    
    SetFXActive(true);
}

void ASTURifleWeaponActor::SetFXActive(bool IsActive)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!IsActive);
        MuzzleFXComponent->SetVisibility(IsActive, true);
    }

    if(FireAudioComponent)
    {
        FireAudioComponent->SetPaused(!IsActive);
    }
}

void ASTURifleWeaponActor::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetVariableVec3(TraceTargetName, TraceEnd);
    }
}