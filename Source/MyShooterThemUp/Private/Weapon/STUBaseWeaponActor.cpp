// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUBaseWeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

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
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeaponActor::StartFire() {}

void ASTUBaseWeaponActor::StopFire() {}

void ASTUBaseWeaponActor::MakeShot() {}

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

bool ASTUBaseWeaponActor::MakeHit(UWorld* World, const FVector& CameraStart, const FVector& CameraEnd, FShotTraceResult& Out) const
{
    if (!World)
        return false;

    FHitResult CameraHit;
    {
        FCollisionQueryParams Params(SCENE_QUERY_STAT(Weapon_CameraTrace), false);
        Params.AddIgnoredActor(GetOwner());
        Params.AddIgnoredActor(this);

        World->LineTraceSingleByChannel(CameraHit, CameraStart, CameraEnd, ECollisionChannel::ECC_Visibility, Params);
    }

    Out.AimPoint = CameraHit.bBlockingHit ? CameraHit.ImpactPoint : CameraEnd;

    const FTransform MuzzleTM = GetMuzzleTM();
    Out.MuzzleStart = MuzzleTM.GetLocation();

    Out.DirFromMuzzle = Out.AimPoint - Out.MuzzleStart;
    if (!IsTraceMuzzleValidate(Out.DirFromMuzzle))
        return false;

    Out.DirFromMuzzle.Normalize();
    Out.MuzzleEnd = Out.MuzzleStart + Out.DirFromMuzzle * TraceMaxDistance;
    {
        FCollisionQueryParams Params(SCENE_QUERY_STAT(Weapon_MuzzleTrace), false);
        Params.AddIgnoredActor(GetOwner());
        Params.AddIgnoredActor(this);

        World->LineTraceSingleByChannel(Out.Hit, Out.MuzzleStart, Out.MuzzleEnd, ECollisionChannel::ECC_Visibility, Params);
    }

    Out.bHit = Out.Hit.bBlockingHit;
    return true;
}

FVector ASTUBaseWeaponActor::GetDirFromMuzzle(const FVector AimPoint) const
{
    return AimPoint - GetMuzzleTM().GetLocation();
}

bool ASTUBaseWeaponActor::IsTraceMuzzleValidate(FVector DirFromMuzzle) const
{
    if (!DirFromMuzzle.Normalize())
        return false;

    const FTransform MuzzleTM = GetMuzzleTM();
    const FVector MuzzleFwd = MuzzleTM.GetRotation().GetForwardVector();

    constexpr float MaxAngleDeg = 89.0f;
    const float CosThr = FMath::Cos(FMath::DegreesToRadians(MaxAngleDeg));
    return FVector::DotProduct(MuzzleFwd, DirFromMuzzle) > CosThr;
}

FTransform ASTUBaseWeaponActor::GetMuzzleTM() const
{
    return WeaponMesh ? WeaponMesh->GetSocketTransform(MuzzleSocketName) : FTransform::Identity;
}

void ASTUBaseWeaponActor::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
        return;
    }
    
    CurrentAmmo.Bullets--;

    if(IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

bool ASTUBaseWeaponActor::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeaponActor::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeaponActor::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips &&
            CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

void ASTUBaseWeaponActor::ChangeClip()
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(LogBaseWeapon, Display, TEXT("------- Change clip -------"));
}

bool ASTUBaseWeaponActor::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ASTUBaseWeaponActor::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
        return false;

    if (IsAmmoFull())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty"));
        CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if(CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added"));
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
            UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now"));
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"));
    }
    return true;
    
}

void ASTUBaseWeaponActor::LogAmmo()
{
    FString AmmoInfo = "Ammo" + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}
