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
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);


// VFX

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UMaterialInstance* Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f);
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};


USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;
};

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 10; // in seconds

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor DefaultTeamColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FLinearColor> TeamsColor;
};