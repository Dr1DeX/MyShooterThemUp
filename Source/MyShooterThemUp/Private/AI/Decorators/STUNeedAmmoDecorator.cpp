// Shoot Them Up Game, All Rights Reserved.


#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogNeedAmmoDecorator, All,All);

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return false;

    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Controller->GetPawn());
    if (!HealthComponent)
        return false;
    
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent)
        return false;
    const bool Result = WeaponComponent->NeedAmmo(WeaponType);
    return Result;
}
