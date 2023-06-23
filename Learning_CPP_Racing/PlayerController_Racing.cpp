// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Racing.h"

void APlayerController_Racing::ChangePoints(int8 Amount)
{
	Points = Points + Amount;
}

void APlayerController_Racing::UpdatePointsVisualisation()
{
}
