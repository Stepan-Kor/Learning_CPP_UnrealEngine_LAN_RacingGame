// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Racing.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API APlayerController_Racing : public APlayerController
{
	GENERATED_BODY()
public:
	void ChangePoints(int8 Amount);
	void UpdatePointsVisualisation();
	int8 Points{ 0 };
	class AGameState_Playing* GetGameState();
protected:
	virtual void BeginPlay() override;
	class AGameState_Playing* GameState;
	UFUNCTION(Server, Reliable)void Server_ChangePoints(int8 NewValue);
	void Server_ChangePoints_Implementation(int8 NewValue);
	UFUNCTION(NetMultiCast, Reliable)void Multi_ChangePoints(int8 NewValue);
	void Multi_ChangePoints_Implementation(int8 NewValue);
};
