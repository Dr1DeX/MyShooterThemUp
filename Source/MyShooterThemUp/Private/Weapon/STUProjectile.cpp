// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
}
