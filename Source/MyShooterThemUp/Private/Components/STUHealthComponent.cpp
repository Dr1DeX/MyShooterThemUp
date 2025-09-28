// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
        return;

    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    if (IsDead())
    {
        StopAutoHeal();
        OnDeath.Broadcast();
        return;
    }

    StopAutoHeal();
    if (bAutoHeal)
    {
        StartAutoHeal();
    }
}

void USTUHealthComponent::HealTick()
{
    if (IsDead())
    {
        StopAutoHeal();
        return;
    }

    const float Amount = HealModifier;

    const float OldHealth = Health;

    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

    if (!FMath::IsNearlyEqual(Health, OldHealth))
    {
        OnHealthChanged.Broadcast(Health);
    }

    if (FMath::IsNearlyEqual(Health, MaxHealth))
    {
        StopAutoHeal();
    }
}

void USTUHealthComponent::StartAutoHeal()
{
    if (!bAutoHeal || IsDead() || Health >= MaxHealth)
        return;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealTick, HealUpdateTime, true, HealDelay);
    }
}

void USTUHealthComponent::StopAutoHeal()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}
