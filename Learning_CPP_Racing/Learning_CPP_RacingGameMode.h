// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "Learning_CPP_RacingGameMode.generated.h"

UCLASS(minimalapi)
class ALearning_CPP_RacingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALearning_CPP_RacingGameMode();
	virtual void RestartPlayer(AController* NewPlayer)override;
protected:
	int8 StartPointCounter{0};
	int16 StartPointsAmount{0};
	TArray <AActor*> StartPoints;
	bool bSearchedStartPoints{false};

	/**
	*	Find all Actors in the world of the specified class. 
	*/
	AActor* FindStartingPointCustom();
};

