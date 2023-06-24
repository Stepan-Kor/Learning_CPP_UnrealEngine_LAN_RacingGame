// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_Playing.h"
#include "Net/UnrealNetwork.h"

UUserWidget* AGameState_Playing::GetScreenWidget()
{
	if (IsValid(ScreenWidget))return ScreenWidget;
	ScreenWidget = CreateWidget(GetWorld(), ScreenWidgetClass, FName("Screen Widget"));
	if (ScreenWidget)ScreenWidget->AddToViewport();
	return ScreenWidget;
}

void AGameState_Playing::IncreasePointsOfPayer(APlayerController_Racing* Controller, int8 Amount)
{
	if (HasAuthority())Multi_IncreasePointsOfPlayer(Controller, Amount);
	else Server_IncreasePointsOfPlayer(Controller,Amount);
}

void AGameState_Playing::Multi_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount)
{
	GetWorld()->GetFirstPlayerController();
}

void AGameState_Playing::Server_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount = 1)
{
	Controller->ChangePoints(Amount);
	Multi_IncreasePointsOfPlayer(Controller, Amount);
}

bool AGameState_Playing::Server_IncreasePointsOfPlayer_Validate(APlayerController_Racing* Controller, int8 Amount = 1)
{
	return true;
}

void AGameState_Playing::OnRep_PointsChanged()
{
	UE_LOG(LogTemp,Warning,TEXT("Game State: broadcasting points update."));
	DelegateList_UpdatePoints.Broadcast(Map_PlayersPoints);
}

void AGameState_Playing::ChangePlayersPoints(APlayerController_Racing* Controller, int8 Diference)
{
	if (!IsValid(Controller))return;
	UE_LOG(LogTemp,Warning,TEXT("Game State: changing %s points by %i."),*Controller->GetName(), Diference);
	Map_PlayersPoints.PlayersPoints.Add(Controller,Diference);
}


void AGameState_Playing::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController_Racing>(GetWorld()->GetFirstLocalPlayerFromController());
	if (IsValid(PlayerController))PlayerController->SetInputMode(FInputModeGameOnly());
	GetScreenWidget();
	//UE_LOG(LogTemp,Warning,TEXT("Game mode playing start playing."));
}

void AGameState_Playing::GetLifetimeReplicatedProps (TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameState_Playing, Map_PlayersPoints);
}


