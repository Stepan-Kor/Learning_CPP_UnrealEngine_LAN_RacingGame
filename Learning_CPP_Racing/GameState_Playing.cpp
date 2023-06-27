// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_Playing.h"
#include "UserWidget_ScreenData.h"
#include "Kismet/GameplayStatics.h"
#include "GameframeWork/PlayerState.h"
#include "Net/UnrealNetwork.h"


void AGameState_Playing::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController_Racing>(GetWorld()->GetFirstLocalPlayerFromController());
	if (IsValid(PlayerController))PlayerController->SetInputMode(FInputModeGameOnly());
	GetScreenWidget();
	//UE_LOG(LogTemp,Warning,TEXT("Game mode playing start playing."));

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameState_Playing::PrintInfo, 1, false);
	SetReplicates(true);
	return;
	UE_LOG(LogTemp, Warning, TEXT("Game State: counting player states..."));

	for (int32 i = 0; i < PlayerArray.Num();i++) {
		UE_LOG(LogTemp, Warning, TEXT("Game State: found player state ID %i"), PlayerArray[i]->PlayerId);
	}
}

UUserWidget* AGameState_Playing::GetScreenWidget()
{
	if (IsValid(ScreenWidget))return ScreenWidget;
	ScreenWidget = CreateWidget(GetWorld(), ScreenWidgetClass, FName("Screen Widget"));
	if (ScreenWidget)ScreenWidget->AddToViewport();
	return ScreenWidget;
	FTimerHandle UnusedHandle;

	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameState_Playing::PrintInfo, 1, false);
}


void AGameState_Playing::Multi_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount)
{
	//GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("Game State: multicast implementation %s."), *GetName());
	//ChangePlayersPoints(Controller, Amount);
}

void AGameState_Playing::Server_IncreasePointsOfPlayer_Implementation(int32 PlayerID, int8 NewValue=0)
{
	Map_PlayersPoints.PlayersPoints.Add(PlayerID, NewValue);
}

bool AGameState_Playing::Server_IncreasePointsOfPlayer_Validate(int32 PlayerStateID, int8 Amount = 1)
{
	return true;
}

void AGameState_Playing::OnRep_PointsChanged()
{
	UE_LOG(LogTemp,Warning,TEXT("Game State: broadcasting points update."));
	//DelegateList_UpdatePoints.Broadcast(Map_PlayersPoints);
}

void AGameState_Playing::ChangePlayersPoints(int32 PlayerStateID, int8 NewValue)
{
	UE_LOG(LogTemp,Warning,TEXT("Game State (%s): changing player(%i) points to %i. And broadcasting...")
		, *GetName(), PlayerStateID, NewValue);
	Server_IncreasePointsOfPlayer(NewValue);
	DelegateList_UpdatePoints.Broadcast(PlayerStateID, NewValue);
}

void AGameState_Playing::PrintInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Game State: counting player states..."));
	
	for (int32 i = 0; i < PlayerArray.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Game State: found player state ID %i"), PlayerArray[i]->PlayerId);
	}
	return;
	TArray<AActor*>  Array_PStates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameState_Playing::StaticClass(), Array_PStates);
	UE_LOG(LogTemp, Error, TEXT("GameState(%s): authority %i,         total gamestates %i, replicates %d."),
		*GetName(), GetLocalRole(), Array_PStates.Num(),GetIsReplicated());
}


void AGameState_Playing::GetLifetimeReplicatedProps (TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(AGameState_Playing, Map_PlayersPoints);
}
