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
};
