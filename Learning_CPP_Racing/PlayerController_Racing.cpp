// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Racing.h"
#include "Kismet/GameplayStatics.h"
#include "GameState_Playing.h"
#include "MyCar.h"
#include "PlayerState_Racing.h"


void APlayerController_Racing::UpdatePointsVisualisation()
{
}

AGameState_Playing* APlayerController_Racing::GetGameState()
{
	if (GameState)return GameState;
	return GameState = Cast<AGameState_Playing>(GetWorld()->GetGameState());
}

void APlayerController_Racing::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController()) { if (GetGameState())GetGameState()->SetOwner(this); }
	SetInputMode(FInputModeGameOnly());
	/*
	TArray <AActor*> Array_Controllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerController::StaticClass(), Array_Controllers);
	UE_LOG(LogTemp, Warning, TEXT("Controller(%s): authority %i,         total controllers %i."),
		*GetName(), GetLocalRole(), Array_Controllers.Num());
	PlayerStateRacing = GetPlayerState<APlayerState_Racing>();
	if (PlayerStateRacing) {
		AMyCar* TempCar = GetPawn<AMyCar>();
		if (TempCar)TempCar->ChangePoints(0);
		else UE_LOG(LogTemp, Warning, TEXT("Controller: car not exist."));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Controller: player state not exist."));*/
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
		UE_LOG(LogTemp, Warning, TEXT("Controller( %s): net multicast empty gamestate pointer."), *GetName());
		return; };
	UE_LOG(LogTemp, Warning, TEXT("Controller( %s): net multicast points change."), *GetName());

}
