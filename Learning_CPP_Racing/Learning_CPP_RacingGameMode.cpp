// Copyright Epic Games, Inc. All Rights Reserved.

#include "Learning_CPP_RacingGameMode.h"
#include "Learning_CPP_RacingPawn.h"
#include "Learning_CPP_RacingHud.h"

ALearning_CPP_RacingGameMode::ALearning_CPP_RacingGameMode()
{
	DefaultPawnClass = ALearning_CPP_RacingPawn::StaticClass();
	HUDClass = ALearning_CPP_RacingHud::StaticClass();
}
