// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"


class UCameraShakeBase;

    UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class MYSHOOTERTHEMUP_API USTUHealthComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return Health <= 0.0f; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const {return Health / MaxHealth;}
    
    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

    bool TryToHealth(int32 HealthAmount);
    bool IsHealthFull() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal")
    bool bAutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", meta = (ClampMin = "0.01", UIMin = "0.05"))
    float HealUpdateTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", meta = (ClampMin = "0.0"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|AutoHeal", meta = (ClampMin = "0.0"))
    float HealModifier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;
    
    virtual void BeginPlay() override;

private:
    float Health = 0.0f;

    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void HealTick();

    void StartAutoHeal();

    void StopAutoHeal();
        void PlayCameraShake();
        
    void ApplyDamage(float Damage, AController* InstigatedBy);

    void SetHealth(float NewHealth);
    void HealUpdate();
        void Killed(AController* KillerController);
};
