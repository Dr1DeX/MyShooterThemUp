// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUReturnMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUReturnMenuWidget, All, All);


void USTUReturnMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(ReturnMenuButton)
    {
        ReturnMenuButton->OnClicked.AddDynamic(this, &USTUReturnMenuWidget::OnReturnMenuButton);
    }
}

void USTUReturnMenuWidget::OnReturnMenuButton()
{
    if(!GetWorld())
        return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!STUGameInstance)
        return;

    if(STUGameInstance->GetMenuLevelName().IsNone())
    {
        UE_LOG(LogSTUReturnMenuWidget, Error, TEXT("Menu Level name is NONE"));
        return;
    }
    UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}