
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Racing.h"
#include "PlayerController_Racing.h"
#include "Kismet/GameplayStatics.h"

#include "MyCar.h"

#include "GameState_Playing.h" 

void APlayerState_Racing::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<AGameState_Playing>();
	PController = GetWorld()->GetFirstPlayerController<APlayerController_Racing>();
	//this->GetPlayerId();
	if (PController) { 
		AMyCar* TempCar = PController->GetPawn<AMyCar>();
		if (TempCar) {
			TempCar->ChangePoints(0);
			//if(TempCar->ScreenWidget)TempCar->ScreenWidget->RefreshPointsFromGameState();
		}
	}
}

void APlayerState_Racing::PrintInfo()
{

	TArray<AActor*>  Array_PStates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerState_Racing::StaticClass(), Array_PStates);
	UE_LOG(LogTemp, Error, TEXT("PlayerState(%s): authority %i,         total player states %i."),
		*GetName(), GetLocalRole(), Array_PStates.Num());
}

APlayerState_Racing::APlayerState_Racing()
{
	//SetReplicates(true);
}

void APlayerState_Racing::ChangePoints(int8 Diference)
{
	Server_ChangePoints(Diference);
}

void APlayerState_Racing::Server_ChangePoints_Implementation(int8 Diference)
{
	Points += Diference;
	Multi_ChangePoints(Points);
}

void APlayerState_Racing::Multi_ChangePoints_Implementation(int8 NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerState(%s): muticast points %i authority %i."),
		*GetName(), Points, GetLocalRole());
	if (!IsValid(GameState))return;
	//if ( PController->HasLocalNetOwner())
	GameState->ChangePlayersPoints(GetPlayerId(),NewValue);
}