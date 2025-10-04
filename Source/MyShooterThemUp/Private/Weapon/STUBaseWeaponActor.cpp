// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUBaseWeaponActor.h"
#include "Components/SkeletalMeshComponent.h"

ASTUBaseWeaponActor::ASTUBaseWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeaponActor::BeginPlay()
{
    Super::BeginPlay();
}
