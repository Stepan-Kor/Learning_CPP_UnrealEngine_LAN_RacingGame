// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Racing.h"
#include "Kismet/GameplayStatics.h"
#include "GameState_Playing.h"
#include "UserWidget_ScreenData.h"
#include "MyCar.h"
#include "PlayerState_Racing.h"


void APlayerController_Racing::BeginPlay()
{
	//UE_LOG(LogTemp, Error, TEXT("PlayerContr: START."));
	Super::BeginPlay();
	if (!IsLocalController()) return;
	SetInputMode(FInputModeGameOnly());
	//GetWidgetScreenData();
	//need to be deleted later
	GetGameState();
	PlayerStateRacing = GetPlayerState<APlayerState_Racing>();
	if (PlayerStateRacing)PlayerStateRacing->SetOwner(this);
	/*
	TArray <AActor*> Array_Controllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), Array_Controllers);
	UE_LOG(LogTemp, Warning, TEXT("Controller(%s): authority %i,         total controllers %i."),
		*GetName(), GetLocalRole(), Array_Controllers.Num());*/
	/*
	if (PlayerStateRacing) {
		AMyCar* TempCar = GetPawn<AMyCar>();
		if (TempCar)TempCar->ChangePoints(0);
		else UE_LOG(LogTemp, Warning, TEXT("Controller: car not exist."));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Controller: player state not exist."));*/
	
}

void APlayerController_Racing::UpdatePointsVisualisation()
{
}

AGameState_Playing* APlayerController_Racing::GetGameState()
{
	if (GameState)return GameState;
	GameState = Cast<AGameState_Playing>(GetWorld()->GetGameState());
	if (GameState) {
		if (!GameState->HasAuthority()) {
			GameState->SetOwner(this);
		}
	}
	return GameState;
}

UUserWidget_ScreenData* APlayerController_Racing::GetWidgetScreenData()
{
	if (WidgetScreenData)return WidgetScreenData;
	WidgetScreenData = Cast<UUserWidget_ScreenData>
			(CreateWidget(GetWorld(), WidgetScreenDataClass, FName("Screen Widget")));
	if (WidgetScreenData) { 
		WidgetScreenData->AddToViewport(); 
		WidgetScreenData->SetPlayerState(PlayerStateRacing);
		PawnCar=GetPawn<AMyCar>();
		if (PawnCar)PawnCar->ScreenWidget = WidgetScreenData;
	}
	return WidgetScreenData;
}

bool APlayerController_Racing::SetPlayerStateRacing(APlayerState_Racing* NewState)
{
	if (PlayerStateRacing)return false;
	if (!IsValid(NewState)) {
		UE_LOG(LogTemp, Warning, TEXT("Controller: Passed empty player state."));
		return false;
	}
	PlayerStateRacing = NewState;
	return true;
}

void APlayerController_Racing::ChangePoints(int8 Amount)
{
	Points = Points + Amount;
	if (!HasAuthority())Server_ChangePoints(Points);
	else Multi_ChangePoints(Points);
}

void APlayerController_Racing::Server_ChangePoints_Implementation(int8 NewValue)
{
	Multi_ChangePoints(Points);
}

void APlayerController_Racing::Multi_ChangePoints_Implementation(int8 NewValue)
{
	if (!GetGameState()) {
		UE_LOG(LogTemp, Warning, TEXT("Controller( %s): net multicast get empty gamestate pointer."), *GetName());
		return; };
	//UE_LOG(LogTemp, Warning, TEXT("Controller( %s): net multicast points change."), *GetName());

}
