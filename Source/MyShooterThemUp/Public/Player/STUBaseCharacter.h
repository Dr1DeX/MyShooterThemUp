// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class ASTUBaseWeaponActor;

UCLASS()
class MYSHOOTERTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASTUBaseCharacter(const FObjectInitializer& ObjInt);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componets")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componets")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componets")
    USTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componets")
    UTextRenderComponent* HealthTextComponent;

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    TSubclassOf<ASTUBaseWeaponActor> WeaponClass;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, category = "Movement")
    bool IsRunn() const;

    UFUNCTION(BlueprintCallable, category = "Movement")
    float GetMovementDirection() const;

private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    void MoveForwarder(float Amount);
    void MoveRighter(float Amount);

    void OnStartRun();
    void OnStopRun();
    void OnDeath();
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    void SpawnWeapon();
};
