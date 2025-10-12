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

    virtual void Fire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Weapon")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    virtual void BeginPlay() override;

    void MakeShot();
    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeHit(UWorld* World, const FVector& AimPoint);
};
