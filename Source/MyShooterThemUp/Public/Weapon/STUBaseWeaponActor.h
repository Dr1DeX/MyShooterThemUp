// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeaponActor.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MYSHOOTERTHEMUP_API ASTUBaseWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeaponActor();

    virtual void StartFire();
    virtual void StopFire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Weapon")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Weapon |Damage")
    float BaseDamage = 25.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Weapon |Damage")
    float HeadshotMultiplier = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components | Weapon |Damage")
    TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

    virtual void BeginPlay() override;

    virtual void MakeShot();

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    void MakeHit(UWorld* World, const FVector& AimPoint);

    void MakeDamage(FHitResult ShotHit, FVector DirFromMuzzle);
};
