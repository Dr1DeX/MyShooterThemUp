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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Weapon")
    USkeletalMeshComponent* WeaponMesh;

    virtual void BeginPlay() override;
};
