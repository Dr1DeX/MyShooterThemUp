// Shoot Them Up Game, All Rights Reserved.


#include "AI/Services/STUChangeWeaponService.h"

#include "AIController.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"


USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
    Interval = 1.0f;
    RandomDeviation = 0.2f;
}


void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
    if (WeaponComponent && LuckyChangeWeapon())
    {
        WeaponComponent->NextWeapon();
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
}

bool USTUChangeWeaponService::LuckyChangeWeapon()
{
    if (Probability <= 0.0f)
    {
        return false;
    }

    if (Probability >= 1.0f)
    {
        return true;
    }

    const float RandomValue = FMath::FRand();
    return RandomValue <= Probability;
}