// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Racing.h"
#include "GameState_Playing.h"


void APlayerController_Racing::UpdatePointsVisualisation()
{
}

void APlayerController_Racing::BeginPlay()
{
	Super::BeginPlay();
	GameState=Cast<AGameState_Playing>(GetWorld()->GetGameState());
	//if(GameState)GameState
	SetReplicates(true);
	UE_LOG(LogTemp,Warning,TEXT("Controller(%s): authority %i."),*GetName(), GetLocalRole());
}

void APlayerController_Racing::ChangePoints(int8 Amount)
{
	Points = Points + Amount;
	if (HasAuthority())Server_ChangePoints(Points);
	else Multi_ChangePoints(Points);
}

void APlayerController_Racing::Server_ChangePoints_Implementation(int8 Diference)
{
	if (!GameState)return;/*
	UE_LOG(LogTemp, Warning, TEXT("Controller( %s): server points change."), *GetName());
	GameState->ChangePlayersPoints(this, Diference);*/
	Multi_ChangePoints(Diference);
}

void APlayerController_Racing::Multi_ChangePoints_Implementation(int8 Diference)
{
	if (!GameState)return;
	FString TempString = this->GetFullName();
	UE_LOG(LogTemp, Warning, TEXT("Controller( %s): net multicast points change."),*TempString);
	GameState->ChangePlayersPoints(this, Diference);
}
