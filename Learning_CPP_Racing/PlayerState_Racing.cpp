
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Racing.h"

void APlayerState_Racing::ChangePoints(int8 Diference)
{
	Server_ChangePoints(Diference);
}

void APlayerState_Racing::Server_ChangePoints_Implementation(int8 Diference)
{
	Points += Diference;
	Multi_ChangePoints(Points);
}

void APlayerState_Racing::Multi_ChangePoints_Implementation(int8 NewValue)
{
	Points = NewValue;
	UE_LOG(LogTemp, Warning, TEXT("PlayerState(%s): muticast points %i authority %i."),
		*GetName(), Points, GetLocalRole());
	DelegateList_UpdatedPoints.Broadcast(NewValue);
}
