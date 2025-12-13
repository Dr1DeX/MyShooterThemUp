// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;

UCLASS()
class MYSHOOTERTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;

protected:
    UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

private:
    UFUNCTION()
    void OnClearPause();
};
