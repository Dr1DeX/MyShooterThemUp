// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"


UCLASS()
class MYSHOOTERTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    FLevelData GetStartupLevel() const { return StartupLevel; }
    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    TArray<FLevelData> GetLevelsData() const { return LevelsData; }
    
    FName GetMenuLevelName() const { return MenuLevelName; }
    
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;
    
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;

private:
    FLevelData StartupLevel;
};
