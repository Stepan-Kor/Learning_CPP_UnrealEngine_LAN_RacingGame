// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Racing.h"
#include "PlayerController_Racing.h"
#include "Kismet/GameplayStatics.h"
#include "MyCar.h"
#include "GameState_Playing.h" 

void APlayerState_Racing::BeginPlay()
{
	//UE_LOG(LogTemp, Error, TEXT("PlayerState: START."));
	Super::BeginPlay();
	PController = GetWorld()->GetFirstPlayerController<APlayerController_Racing>();
	//this->GetPlayerId();
	if (PController) {
		if (!PController->HasLocalNetOwner()) return; 
		if (HasAuthority())SetReplicates(true);
		else SetOwner(PController);
		PController->SetPlayerStateRacing(this);
		PController->GetWidgetScreenData();
		GameState = GetWorld()->GetGameState<AGameState_Playing>();
		if (GameState) {
			GameState->GetPlayerStateRacing(PController, this);
			//ChangePoints();
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerState_Racing::CheckPoints, 1, false);
		}
		else UE_LOG(LogTemp, Error, TEXT("PlayerState: cant get game state."));
	}
	else {UE_LOG(LogTemp, Error, TEXT("PlayerState: cant get player controller."));}

}

void APlayerState_Racing::PrintInfo()
{
	TArray<AActor*>  Array_PStates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerState_Racing::StaticClass(), Array_PStates);
	/*UE_LOG(LogTemp, Error, TEXT("PlayerState(%s): authority %i,         total player states %i."),
		*GetName(), GetLocalRole(), Array_PStates.Num());*/
}

APlayerState_Racing::APlayerState_Racing()
{
	
}

APlayerController_Racing* APlayerState_Racing::GetControllerRacing()
{
	if (PController)return PController;
	PController = GetWorld()->GetFirstPlayerController<APlayerController_Racing>();
	return PController;
}

AGameState_Playing* APlayerState_Racing::GetGameState()
{
	if (GameState)return GameState;
	GameState = GetWorld()->GetGameState<AGameState_Playing>();
	return GameState;
}

void APlayerState_Racing::ChangePoints(int8 Diference)
{/*
	FString TempString{"empty"};
	if (GetOwner())TempString = GetOwner()->GetName();
	UE_LOG(LogTemp,Warning,TEXT("PlayerState(auth %d, onwer - %s): something changing points by %i."),
		HasAuthority(),*TempString,Diference);*/
		/*
	UE_LOG(LogTemp, Warning, TEXT("PlayerState(%s): authority %i, has owner %d. call for server."),
		*GetName(), GetLocalRole(),IsValid(GetOwner()));*/
	Server_ChangePoints(Diference);
}

void APlayerState_Racing::CheckPoints()
{
	ChangePoints();
}

void APlayerState_Racing::Server_ChangePoints_Implementation(int8 Diference)
{
	Points += Diference;
	//if(GetGameState())GameState->ChangePlayersPoints(GetPlayerId(), Points);
	//else { UE_LOG(LogTemp, Warning, TEXT("PlayerState(serv): cant get Gamestate.")); }
	Multi_ChangePoints(Points);
}

void APlayerState_Racing::Multi_ChangePoints_Implementation(int8 NewValue)
{
	/*UE_LOG(LogTemp, Warning, TEXT("PlayerState(%s): muticast points %i authority %i."),
		*GetName(), Points, GetLocalRole());*/
	if(GetGameState())GameState->ChangePlayersPoints(GetPlayerId(),NewValue);
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerState(%s): cant get gamestate."),*GetName());
	}
	//DelegateList_UpdatedPoints.Broadcast(GetPlayerId(),NewValue);
	//if (!IsValid(GameState))return;
	//if ( PController->HasLocalNetOwner())
	//GameState->ChangePlayersPoints(GetPlayerId(),NewValue);
}