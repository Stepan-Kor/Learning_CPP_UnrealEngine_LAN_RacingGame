// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_PlayersPoints.h"

void UUserWidget_PlayersPoints::UpdateText(int8 NewPoints)
{
	if (!IsValid(TextBlock_NameAndPoints))return;
	//FString TempString=PointsStruct.PlayersPoints.
	//TextBlock_NameAndPoints->SetText(NewText);
	FString TempString{};
	TempString.Append(PlayerController->GetName());
	TempString.Append("\t");
	TempString.Append(FString::FromInt(NewPoints));
	TextBlock_NameAndPoints->SetText(FText::FromString(TempString));
}

void UUserWidget_PlayersPoints::InitializePointsWidget(APlayerController_Racing* Controller,int8 CurrentPoints)
{
	PlayerController = Controller;
	if (!Controller || !IsValid(TextBlock_NameAndPoints))return;
	FString TempString{};
	TempString.Append(Controller->GetName());
	TempString.Append("\t");
	TempString.Append(FString::FromInt(CurrentPoints));
	TextBlock_NameAndPoints->SetText(FText::FromString(TempString));
}