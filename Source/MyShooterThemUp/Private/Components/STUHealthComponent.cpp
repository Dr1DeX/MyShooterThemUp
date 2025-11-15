// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToHealth(int32 HealthAmount)
{
    if (HealthAmount <= 0.0f || IsDead() || FMath::IsNearlyEqual(Health, MaxHealth))
        return false;

    const float OldHealth = Health;
    Health = FMath::Clamp(Health + HealthAmount, 0.0f, MaxHealth);

    if (!FMath::IsNearlyEqual(Health, OldHealth))
    {
        OnHealthChanged.Broadcast(Health);
    }

    return !FMath::IsNearlyEqual(Health, OldHealth);
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    
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

    ApplyDamage(Damage, InstigatedBy);
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

void USTUHealthComponent::ApplyDamage(float Damage, AController*)
{
    if (Damage <= 0.f || IsDead())
        return;

    const float OldHealth = Health;
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

    if (!FMath::IsNearlyEqual(Health, OldHealth))
    {
        OnHealthChanged.Broadcast(Health);
    }

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
