// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUReturnMenuWidget.generated.h"

class UButton;

UCLASS()
class MYSHOOTERTHEMUP_API USTUReturnMenuWidget : public UUserWidget
{
	GENERATED_BODY()

    UPROPERTY(meta = (BindWidget))
    UButton* ReturnMenuButton;

protected:
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnReturnMenuButton();
};
