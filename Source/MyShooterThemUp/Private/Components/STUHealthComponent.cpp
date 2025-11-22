// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToHealth(int32 HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);
    return true;
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    
    SetHealth(MaxHealth);

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

    const float NewHealth = Health + HealModifier;
    SetHealth(NewHealth);

    if (IsHealthFull())
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

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;
    
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController*)
{
    if (Damage <= 0.f || IsDead())
        return;

    const float NewHealth = Health - Damage;
    SetHealth(NewHealth);

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
    PlayCameraShake();
}
