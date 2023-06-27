// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_PlayersPoints.h"

void UUserWidget_PlayersPoints::UpdateText(int8 NewPoints)
{
	if (!IsValid(TextBlock_NameAndPoints))return;
	FString TempString{ "Player(" };
	TempString.Append(FString::FromInt(PlayerID));
	TempString.Append(")\t points: ");
	TempString.Append(FString::FromInt(NewPoints));
	TempString.Append(".");
	TextBlock_NameAndPoints->SetText(FText::FromString(TempString));
}

void UUserWidget_PlayersPoints::InitializePointsWidget(int32 LPlayerID,int8 CurrentPoints)
{
	//PlayerController = Controller;
	PlayerID = LPlayerID;
	UpdateText(CurrentPoints);
}