// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_Playing.h"
#include "UserWidget_ScreenData.h"
#include "Kismet/GameplayStatics.h"
#include "GameframeWork/PlayerState.h"
#include "PlayerState_Racing.h"


void AGameState_Playing::BeginPlay()
{
	//UE_LOG(LogTemp, Error, TEXT("GameState: START."));
	Super::BeginPlay();
	//GetPlayerStateRacing();
	//FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameState_Playing::PrintInfo, 0.5, false);
	//if (IsValid(PlayerController))PlayerController->SetInputMode(FInputModeGameOnly());
	//UE_LOG(LogTemp,Warning,TEXT("Game mode playing start playing."));
	/*
	for (int32 i = 0; i < PlayerArray.Num();i++) {
		UE_LOG(LogTemp, Warning, TEXT("Game State: found player state ID %i"), PlayerArray[i]->PlayerId);
	}*/
}


void AGameState_Playing::Server_TestCall_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Game State: Server received test call."));
}


void AGameState_Playing::Multi_IncreasePointsOfPlayer_Implementation(int32 PlayerID, int8 Amount)
{
	//GetWorld()->GetFirstPlayerController();
	DelegateList_PlayersPointsUpdated.Broadcast(PlayerID,Amount);
	return;
	int32 ID{0};
	if (PlayerController) { 
		auto PS =PlayerController->GetPlayerState<APlayerState_Racing>(); 
		if (PS)ID=PS->GetPlayerId();
	}
	UE_LOG(LogTemp, Warning, TEXT("Game State(%d): multicast implementation. String: %s"),
		HasAuthority(),*ReplicatedString);
	//ChangePlayersPoints(Controller, Amount);
	//DelegateList_UpdatePoints.Broadcast(PlayerID, Amount);
	for (auto A : Map_PlayersPoints) {
		UE_LOG(LogTemp,Warning,TEXT("Game State(%d): have ponts pair %i : %i."),
			HasAuthority(), A.PlayerID,A.PlayersPoints);
	}
}



void AGameState_Playing::OnRep_PointsChanged()
{
	
	if (!bPointsWasReplicated) 	bPointsWasReplicated = true;
	DelegateList_UpdatePoints.Broadcast(Map_PlayersPoints);	
	/*	UE_LOG(LogTemp,Warning,TEXT
	("Game State(%d): OnRep function called\n\t\tstring:%s, amount of ints:%i, amount of pairs %i"),
		HasAuthority(),*ReplicatedString, ReplicatedTestInt.Num(), Map_PlayersPoints.Num());*/
}

bool AGameState_Playing::GetPlayerStateRacing(APlayerController_Racing* LPlayerController,
	APlayerState_Racing* LPlayerState)
{
	if (PlayerState)return true;
	//UE_LOG(LogTemp, Warning, TEXT("Game State: setting of pl state and contr."));
	if (!PlayerController) {
		if (LPlayerController == nullptr)PlayerController = Cast<APlayerController_Racing>(GetWorld()->GetFirstLocalPlayerFromController());
		else PlayerController = LPlayerController;
	}
	if (LPlayerState != nullptr)PlayerState = LPlayerState;
	else if (PlayerController)PlayerState = PlayerController->GetPlayerState<APlayerState_Racing>();
	if (HasAuthority()) {
		SetReplicates(true);
		bPointsWasReplicated = true;
	}
	else {
		if (IsValid(PlayerController)) {
			SetOwner(PlayerController);
		}
		else UE_LOG(LogTemp, Warning, TEXT("Game State: cant set owner - no controller."));
		//UE_LOG(LogTemp, Warning, TEXT("Game State: calling server."));
		//FTimerHandle UnusedHandle;
		//GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameState_Playing::Server_TestCall, 3.5, false);
	}
	return IsValid(PlayerState);
}

void AGameState_Playing::ChangePlayersPoints(int32 PlayerStateID, int8 NewValue)
{	/* UE_LOG(LogTemp,Warning,TEXT("Game State (%s): changing player(%i) points to %i. And broadcasting...")
		, *GetName(), PlayerStateID, NewValue);*/
	if(HasAuthority()){
		bool bChanged = false;
		for (int8 i = 0; i < Map_PlayersPoints.Num(); i++) {
			if (Map_PlayersPoints[i].PlayerID == PlayerStateID) {
				Map_PlayersPoints[i].PlayersPoints = NewValue;
				bChanged = true;
				break;
			}
		}
		if (!bChanged) {
			FJustPointsMap TempMap{ PlayerStateID,NewValue };
			Map_PlayersPoints.Add(TempMap);
			//Map_PlayersPoints.Add({ PlayerStateID,NewValue });
		}
		DelegateList_UpdatePoints.Broadcast(Map_PlayersPoints);
		/*
		FString TempString{ "empty" };
		if (GetOwner())TempString = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Game State(auth - %d, owner:%s): points map changed %i - %i."),
			HasAuthority(), *TempString, PlayerStateID, NewValue);*/
	}
	DelegateList_PlayersPointsUpdated.Broadcast(PlayerStateID, NewValue);
	return;
}

void AGameState_Playing::PrintInfo()
{
	if(GetPlayerStateRacing())PlayerState->ChangePoints(0);
	else UE_LOG(LogTemp, Warning, TEXT("Game State: dont have player state."));
	/*
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Game State(server): server send net multi call."));
		//ReplicatedString = "Server new string";
		//ReplicatedTestInt.Add(55);
		TArray <FJustPointsMap> TempArray;
		for (int8 i = 3; i < 14; i++) { 
		TempMap.PlayerID = i; TempMap.PlayersPoints = i; TempArray.Add(TempMap); };
		Map_PlayersPoints=TempArray;*/
		/*
		FJustPointsMap TempMap(13, 13);
		Map_PlayersPoints.Add(TempMap);
		//Multi_IncreasePointsOfPlayer(99, 99);
	}*/
	/*else {
		UE_LOG(LogTemp, Warning, TEXT("Game State(client): client send net server call."));
		Server_TestCall();
	}
	TArray <AActor*> ActorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AGameState_Playing::StaticClass(), ActorsArray);
	//Multi_IncreasePointsOfPlayer(99,99);
	//Map_PlayersPoints.PlayersPoints.Add(99,99);
	//const FString TempString= GetRolePropertyName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Game State (%s): total amount of gamestates %i"),
		*GetRolePropertyName().ToString(), ActorsArray.Num());
	for (int32 i = 0; i < PlayerArray.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Game State: found player state ID %i"), PlayerArray[i]->PlayerId);
	}
	return;
	TArray<AActor*>  Array_PStates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameState_Playing::StaticClass(), Array_PStates);
	UE_LOG(LogTemp, Error, TEXT("GameState(%s): authority %i,         total gamestates %i, replicates %d."),
		*GetName(), GetLocalRole(), Array_PStates.Num(),GetIsReplicated());*/
}


TArray <FJustPointsMap>* AGameState_Playing::GetSavedPoints(){return &Map_PlayersPoints;}

void AGameState_Playing::GetLifetimeReplicatedProps (TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameState_Playing, Map_PlayersPoints);
	DOREPLIFETIME(AGameState_Playing, ReplicatedString);
	DOREPLIFETIME(AGameState_Playing, ReplicatedTestInt);
}
