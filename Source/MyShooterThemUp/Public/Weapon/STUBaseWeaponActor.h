// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeaponActor.generated.h"

class USkeletalMeshComponent;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components | Weapon |Damage")
    TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};
    
    virtual void BeginPlay() override;

    virtual void MakeShot();

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    bool MakeHit(UWorld* World, const FVector& CameraStart, const FVector& CameraEnd, FShotTraceResult& Out) const;

    FVector GetDirFromMuzzle(const FVector AimPoint) const;
    bool IsTraceMuzzleValidate(FVector DirFromMuzzle) const;
    FTransform GetMuzzleTM() const;

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void ChangeClip();
    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
