// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeaponActor.generated.h"


class USkeletalMeshComponent;


UCLASS()
class MYSHOOTERTHEMUP_API ASTUBaseWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeaponActor();

    FOnClipEmptySignature OnClipEmpty;
    
    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();
    bool CanReload() const;

    FWeaponUIData GetUIData() const {return UIData;}
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

    bool TryToAddAmmo(int32 ClipsAmount);
    
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;
    
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
    void LogAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    bool IsAmmoFull() const;
    

private:
    FAmmoData CurrentAmmo;
};
