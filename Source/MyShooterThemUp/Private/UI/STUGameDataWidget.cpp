// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUGameDataWidget.h"

#include "STUPlayerState.h"
#include "MyShooterThemUp/STUGameModeBase.h"

int32 USTUGameDataWidget::GetKillsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRound() : 0;
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundNum() : 0;
}

int32 USTUGameDataWidget::GetRoundsSecondsRemaining() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundCountDown() : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr; 
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}