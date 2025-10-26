// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeaponActor.h"
#include "STURifleWeaponActor.generated.h"

UCLASS()
class MYSHOOTERTHEMUP_API ASTURifleWeaponActor : public ASTUBaseWeaponActor
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BaseDamage = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float HeadshotMultiplier = 2.0f;

private:
    FTimerHandle ShotTimerHandle;

    void MakeDamage(const FHitResult& ShotHit, const FVector& DirFromMuzzle);
};
