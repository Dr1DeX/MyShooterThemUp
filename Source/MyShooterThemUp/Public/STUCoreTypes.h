#pragma once
#include "STUCoreTypes.generated.h"

// weapon

class ASTUBaseWeaponActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeaponActor*);



USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

USTRUCT()
struct FShotTraceResult
{
    GENERATED_BODY()


    bool bHit = false;

    FVector MuzzleStart = FVector::ZeroVector;
    FVector MuzzleEnd = FVector::ZeroVector;

    FVector DirFromMuzzle = FVector::ForwardVector;

    FVector AimPoint = FVector::ZeroVector;

    FHitResult Hit;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeaponActor> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};


USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairIcon;
    
};

// health
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
