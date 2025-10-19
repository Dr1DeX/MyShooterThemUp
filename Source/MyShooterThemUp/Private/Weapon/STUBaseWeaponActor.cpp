// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUBaseWeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeaponActor::ASTUBaseWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

void ASTUBaseWeaponActor::StartFire()
{
}

void ASTUBaseWeaponActor::StopFire()
{
}

void ASTUBaseWeaponActor::MakeShot()
{
}

APlayerController* ASTUBaseWeaponActor::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeaponActor::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto PC = GetPlayerController();
    if (!PC)
        return false;

    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

bool ASTUBaseWeaponActor::GetTraceData(FVector& CameraTraceStart, FVector& CameraTraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    CameraTraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    CameraTraceEnd = CameraTraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTUBaseWeaponActor::MakeHit(UWorld* World, const FVector& AimPoint)
{
    const FTransform MuzzleTM = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector MuzzleLoc = MuzzleTM.GetLocation();
    const FVector MuzzleFwd = MuzzleTM.GetRotation().GetForwardVector();

    FVector DirFromMuzzle = AimPoint - MuzzleLoc;
    if (!DirFromMuzzle.Normalize())
        return;

    constexpr float MaxAngleDeg = 89.0f;
    const float cosThr = FMath::Cos(FMath::DegreesToRadians(MaxAngleDeg));
    if (FVector::DotProduct(MuzzleFwd, DirFromMuzzle) <= cosThr)
        return;

    const FVector ShotStart = MuzzleLoc;
    const FVector ShotEnd = ShotStart + DirFromMuzzle * TraceMaxDistance;

    FCollisionQueryParams ShotParams(SCENE_QUERY_STAT(Weapon_ShotTrace), false);
    ShotParams.AddIgnoredActor(GetOwner());
    ShotParams.AddIgnoredActor(this);

    FHitResult ShotHit;
    World->LineTraceSingleByChannel(ShotHit, ShotStart, ShotEnd, ECollisionChannel::ECC_Visibility, ShotParams);

    if (ShotHit.bBlockingHit)
    {
        DrawDebugLine(World, ShotStart, ShotHit.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(World, ShotHit.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        MakeDamage(ShotHit, DirFromMuzzle);
    }
    else
    {
        DrawDebugLine(World, ShotStart, ShotEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

void ASTUBaseWeaponActor::MakeDamage(FHitResult ShotHit, FVector DirFromMuzzle)
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
        InstigatorController,                              // Who apply to damage
        this,                                              // Damage Causer(weapon)
        DamageType                                         // Damage Type
    );
}