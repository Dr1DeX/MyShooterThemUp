// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeaponActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYSHOOTERTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    void StartFire();
    void StopFire();
    void NextWeapon();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeaponActor>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

private:
    UPROPERTY()
    ASTUBaseWeaponActor* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeaponActor*> Weapons;

    int32 CurrentWeaponIndex = 0;

    void SpawnWeapons();
    void AttachWeaponToSocket(ASTUBaseWeaponActor* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);
};
