// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUGameOverWidget.h"

#include "STUPlayerStatRowWidget.h"
#include "STUUtils.h"
#include "MyShooterThemUp/STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OneMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }
    if(ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if(!GetWorld() || !PlayerStatBox)
        return;

    PlayerStatBox->ClearChildren();
    
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState)
            continue;

        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if(!PlayerStatRowWidgetClass)
            continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}