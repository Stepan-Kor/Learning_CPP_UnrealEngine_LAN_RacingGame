// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameState_Playing.h"
#include "UserWidget_PlayersPoints.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API UUserWidget_PlayersPoints : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_NameAndPoints;
public:
	void UpdateText(int8 NewPoints);
	APlayerController_Racing* PlayerController;
	void InitializePointsWidget(APlayerController_Racing* Controller, int8 CurrentPoints);
};
