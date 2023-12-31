// Copyright Epic Games, Inc. All Rights Reserved.

#include "Learning_CPP_RacingGameMode.h"
#include "Learning_CPP_RacingPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetArrayLibrary.h"
#include "GameState_Playing.h"
#include "PlayerState_Racing.h"
#include "Learning_CPP_RacingHud.h"

void ALearning_CPP_RacingGameMode::StartPlay()
{
	//UE_LOG(LogTemp, Error, TEXT("GameMode: START."));
	Super::StartPlay();
	GameStateRacing = GetGameState<AGameState_Playing>();
}

ALearning_CPP_RacingGameMode::ALearning_CPP_RacingGameMode()
{
	DefaultPawnClass = ALearning_CPP_RacingPawn::StaticClass();
	HUDClass = ALearning_CPP_RacingHud::StaticClass();
}

void ALearning_CPP_RacingGameMode::RestartPlayer(AController* NewPlayer)
{
	//UE_LOG(LogTemp,Warning,TEXT("GameMode: Custom Restart player called."));
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	//AActor* StartSpot = FindPlayerStart(NewPlayer);	original
	AActor* StartSpot = FindStartingPointCustom();

	// If a start spot wasn't found,
	if (StartSpot == nullptr)
	{
		UE_LOG(LogGameMode, Warning, TEXT("Game mode: start point was not found."));
		// Check for a previously assigned spot
		if (NewPlayer->StartSpot != nullptr)
		{
			StartSpot = NewPlayer->StartSpot.Get();
			UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
		}
	}
	//UE_LOG(LogGameMode, Warning, TEXT("Game mode: controller to spawn valid %d"), IsValid(NewPlayer));
	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

void ALearning_CPP_RacingGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!NewPlayer || !GameStateRacing)return;
	UE_LOG(LogTemp,Warning,TEXT("Game Mode: Player %s Post login."), *NewPlayer->GetName());
	APlayerState* PLST = NewPlayer->GetPlayerState<APlayerState>();
	if (!PLST)return;
	GameStateRacing->PlayerDidEnter(PLST->GetPlayerId());
}


void ALearning_CPP_RacingGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	APlayerState* PlayerStateTemp = Exiting->GetPlayerState<APlayerState>();
	if (IsValid(PlayerStateTemp) && IsValid(GameStateRacing)) {
		GameStateRacing->PlayerDidExit(PlayerStateTemp->GetPlayerId());
	}
}

AActor* ALearning_CPP_RacingGameMode::FindStartingPointCustom()
{
	//UE_LOG(LogGameMode, Warning, TEXT("Game mode: custom looking for start point."));
	if (!bSearchedStartPoints) {
		//UKismetSystemLibrary
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPoints);
		bSearchedStartPoints = true;
		StartPointsAmount=StartPoints.Num();
	}
	if (StartPointsAmount < 1)return nullptr;
	StartPointCounter = (StartPointCounter + 1) % StartPointsAmount;

	/*UE_LOG(LogGameMode, Warning, TEXT("Game mode: start points found %i. Used this %s")
		, StartPointsAmount, *StartPoints[StartPointCounter]->GetName());*/
	return StartPoints[StartPointCounter];
}