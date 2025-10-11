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
}

void ASTUBaseWeaponActor::Fire()
{
    UE_LOG(LogBaseWeapon, Warning, TEXT("Fire!"));

    MakeShot();
}

void ASTUBaseWeaponActor::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World)
        return;

    const auto OwnerCharacter = Cast<ACharacter>(GetOwner());

    if (!OwnerCharacter)
        return;

    const auto PC = OwnerCharacter->GetController<APlayerController>();

    if (!PC)
        return;

    FVector ViewLocation;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FTransform MuzzleTM = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector MuzzleLocation = MuzzleTM.GetLocation();
    const FVector MuzzleForward = MuzzleTM.GetRotation().GetForwardVector();

    const FVector CameraTraceStart = ViewLocation;
    const FVector CameraTraceEnd = CameraTraceStart + ViewRotation.Vector() * TraceMaxDistance;

    FCollisionQueryParams CameraParams(SCENE_QUERY_STAT(Weapon_AimTrace), false);
    CameraParams.AddIgnoredActor(this);
    CameraParams.AddIgnoredActor(OwnerCharacter);

    FHitResult AimHit;
    World->LineTraceSingleByChannel(AimHit, CameraTraceStart, CameraTraceEnd, ECollisionChannel::ECC_Visibility);

    const FVector AimPoint = AimHit.bBlockingHit ? AimHit.ImpactPoint : CameraTraceEnd;

    FVector DirFromMuzzle = AimPoint - MuzzleLocation;
    if (!DirFromMuzzle.Normalize())
        return;


    constexpr float MaxAngleDeg = 89.0f;
    const float CosThreshhold = FMath::Cos(FMath::DegreesToRadians(MaxAngleDeg));
    const float CosAngle = FVector::DotProduct(MuzzleForward, DirFromMuzzle);
    if (CosAngle <= CosThreshhold)
        return;

    const FVector ShotStart = MuzzleLocation;
    const FVector ShotEnd = ShotStart + DirFromMuzzle * TraceMaxDistance;

    FCollisionQueryParams ShotParams(SCENE_QUERY_STAT(Weapon_ShotTrace), false);
    ShotParams.AddIgnoredActor(this);
    ShotParams.AddIgnoredActor(OwnerCharacter);

    FHitResult ShotHit;
    World->LineTraceSingleByChannel(ShotHit, ShotStart, ShotEnd, ECollisionChannel::ECC_Visibility, ShotParams);

    if (ShotHit.bBlockingHit)
    {
        DrawDebugLine(World, ShotStart, ShotHit.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(World, ShotHit.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    }
    else
    {
        DrawDebugLine(World, ShotStart, ShotEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}
