// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UserWidget_ScreenData.h"
#include "GameState_Playing.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API AGameState_Playing : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)TSubclassOf<UUserWidget> ScreenWidgetClass=UUserWidget_ScreenData::StaticClass();
	UUserWidget* GetScreenWidget();
protected:
	UUserWidget* ScreenWidget;
	APlayerController* PlayerController;
	virtual void BeginPlay()override;
	
	
};
