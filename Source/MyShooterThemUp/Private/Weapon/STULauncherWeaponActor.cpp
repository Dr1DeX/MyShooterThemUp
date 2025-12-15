// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STULauncherWeaponActor.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeaponActor::StartFire()
{
    MakeShot();
}

void ASTULauncherWeaponActor::MakeShot()
{

    UWorld* World = GetWorld();
    if (!World)
        return;

    if (IsAmmoEmpty())
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        return;
    }
    
    FVector CameraStart, CameraEnd;
    if (!GetTraceData(CameraStart, CameraEnd))
        return;

    FShotTraceResult Shot;
    if (!MakeHit(World, CameraStart, CameraEnd, Shot))
        return;

    const FVector EndPoint = Shot.bHit ? Shot.Hit.ImpactPoint : Shot.MuzzleEnd;
    const FVector Direction = (EndPoint - GetMuzzleTM().GetLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleTM().GetLocation());
    ASTUProjectile* Projectile = World->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
    SpawnMuzzleFX();
    UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}
