// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class MYSHOOTERTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInt);

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componets")
    USTUHealthComponent* HealthComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componets")
    USTUWeaponComponent* WeaponComponent;

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

public:
    virtual void Tick(float DeltaTime) override;
    
    UFUNCTION(BlueprintCallable, category = "Movement")
    virtual bool IsRunn() const;

    UFUNCTION(BlueprintCallable, category = "Movement")
    float GetMovementDirection() const;

    virtual void OnDeath();

    void SetPlayerColor(const FLinearColor& Color);

private:
    void OnHealthChanged(float Health, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
