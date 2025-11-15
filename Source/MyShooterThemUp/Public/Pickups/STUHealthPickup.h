// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"


UCLASS()
class MYSHOOTERTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "50.0"))
    int32 HealthAmount = 50;
    
private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
	
};
