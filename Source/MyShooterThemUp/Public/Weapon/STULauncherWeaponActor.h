// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeaponActor.h"
#include "STULauncherWeaponActor.generated.h"

class ASTUProjectile;
class USoundCue;

UCLASS()
class MYSHOOTERTHEMUP_API ASTULauncherWeaponActor : public ASTUBaseWeaponActor
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* NoAmmoSound;
    
    virtual void MakeShot() override;
};
