// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

#define LOCTEXT_NAMESPACE "STUHUD"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.0f;

    const auto HealthComponent = Player->FindComponentByClass<USTUHealthComponent>();
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return false;

    const auto WeaponComponent = Player->FindComponentByClass<USTUWeaponComponent>();
    return WeaponComponent ? WeaponComponent->GetWeaponUIData(UIData) : false;
}

FText USTUPlayerHUDWidget::GetAmmoText() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return LOCTEXT("AmmoNone", "0 / 0");

    const auto WC = Player->FindComponentByClass<USTUWeaponComponent>();
    
    const int32 Bullets = WC->GetCurrentBullets();
    
    if (WC->IsCurrentInfinite())
    {
        return FText::Format(
            LOCTEXT("AmmoInfinitePattern", "{0} / ∞"),
            FText::AsNumber(Bullets)
        );
    }

    const int32 Clips = WC->GetCurrentClips();
    return FText::Format(
        LOCTEXT("AmmoFinitePattern", "{0} / {1}"),
        FText::AsNumber(Bullets),
        FText::AsNumber(Clips)
    );
    
}

