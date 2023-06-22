// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_Playing.h"

UUserWidget* AGameState_Playing::GetScreenWidget()
{
	if (IsValid(ScreenWidget))return ScreenWidget;
	ScreenWidget = CreateWidget(GetWorld(), ScreenWidgetClass, FName("Screen Widget"));
	if (ScreenWidget)ScreenWidget->AddToViewport();
	return ScreenWidget;
}

void AGameState_Playing::BeginPlay()
{
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))PlayerController->SetInputMode(FInputModeGameOnly());
	GetScreenWidget();
	//UE_LOG(LogTemp,Warning,TEXT("Game mode playing start playing."));
}
