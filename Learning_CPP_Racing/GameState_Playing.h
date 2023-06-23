// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UserWidget_ScreenData.h"
#include "PlayerController_Racing.h"
#include "GameState_Playing.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API AGameState_Playing : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UUserWidget* ScreenWidget;
	APlayerController_Racing* PlayerController;
	virtual void BeginPlay()override;
public:
	UPROPERTY(EditDefaultsOnly)TSubclassOf<UUserWidget> ScreenWidgetClass = UUserWidget_ScreenData::StaticClass();
	UUserWidget* GetScreenWidget();
	void IncreasePointsOfPayer(APlayerController_Racing* Controller,int8 Amount=1);
	UFUNCTION(NetMultiCast, Reliable)void Multi_IncreasePointsOfPlayer(APlayerController_Racing* Controller, int8 Amount = 1);
	void Multi_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount);
	UFUNCTION(Server,Reliable,WithValidation)void Server_IncreasePointsOfPlayer(APlayerController_Racing* Controller, int8 Amount = 1);
	void Server_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount );
	bool Server_IncreasePointsOfPlayer_Validate(APlayerController_Racing* Controller, int8 Amount );
};
