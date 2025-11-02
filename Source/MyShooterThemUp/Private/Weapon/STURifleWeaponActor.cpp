// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STURifleWeaponActor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

void ASTURifleWeaponActor::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeaponActor::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeaponActor::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
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

    const FVector EndPoint = Shot.bHit ? Shot.Hit.ImpactPoint : Shot.MuzzleEnd;
    DrawDebugLine(World, Shot.MuzzleStart, EndPoint, FColor::Red, false, 3.0f, 0, 3.0f);

    if (Shot.bHit)
    {
        DrawDebugSphere(World, Shot.Hit.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        MakeDamage(Shot.Hit, Shot.DirFromMuzzle);
    }
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