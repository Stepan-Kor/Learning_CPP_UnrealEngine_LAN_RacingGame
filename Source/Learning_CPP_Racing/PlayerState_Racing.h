// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Racing.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateTypePointsUpdated,int32,int8);
UCLASS()
class LEARNING_CPP_RACING_API APlayerState_Racing : public APlayerState
{
	GENERATED_BODY()
public:
	APlayerState_Racing();
	class APlayerController_Racing* GetControllerRacing();
	void ChangePoints(int8 Diference=0);
	FDelegateTypePointsUpdated DelegateList_UpdatedPoints;
protected:
	void CheckPoints();
	int8 Points{0};
	class AGameState_Playing* GameState;
	class AGameState_Playing* GetGameState();
	class APlayerController_Racing* PController;
	UFUNCTION(Server,Reliable)void Server_ChangePoints(int8 Diference);
	void Server_ChangePoints_Implementation(int8 Diference);
	UFUNCTION(NetMultiCast,Reliable)void Multi_ChangePoints(int8 NewValue);
	void Multi_ChangePoints_Implementation(int8 NewValue);
	virtual void BeginPlay()override;
	void PrintInfo();
};