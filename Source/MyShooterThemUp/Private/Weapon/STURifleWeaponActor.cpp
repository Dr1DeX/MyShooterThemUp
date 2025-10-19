// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STURifleWeaponActor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
    if (!World)
        return;

    FVector CameraStart, CameraEnd;
    if (!GetTraceData(CameraStart, CameraEnd))
        return;

    FCollisionQueryParams CameraParams(SCENE_QUERY_STAT(Weapon_AimTrace), false);
    CameraParams.AddIgnoredActor(GetOwner());
    CameraParams.AddIgnoredActor(this);

    FHitResult CameraHit;
    World->LineTraceSingleByChannel(CameraHit, CameraStart, CameraEnd, ECollisionChannel::ECC_Visibility, CameraParams);
    const FVector AimPoint = CameraHit.bBlockingHit ? CameraHit.ImpactPoint : CameraEnd;

    MakeHit(World, AimPoint);
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