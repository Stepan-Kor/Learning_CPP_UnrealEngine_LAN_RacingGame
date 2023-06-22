// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_ScreenData.h"


bool UUserWidget_ScreenData::Initialize()
{
	Super::Initialize();
	if (Button_Resume)Button_Resume->OnClicked.AddDynamic(this,&UUserWidget_ScreenData::Resume);
	PlayerController = GetWorld()->GetFirstPlayerController();
	return true;
}

void UUserWidget_ScreenData::Resume()
{
	if (!IsValid(PlayerController))return;
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(false);
	VerticalBox_Buttons->SetVisibility(ESlateVisibility::Hidden);
}

void UUserWidget_ScreenData::Pause()
{
	if (!IsValid(PlayerController))return;
	VerticalBox_Buttons->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}
