// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerController_Racing.h"
#include "Blueprint/UserWidget.h"
#include "GameState_Playing.generated.h"

/**
 */
USTRUCT() struct FJustPointsMap {
	GENERATED_BODY()
	TMap<APlayerController_Racing*, int8> PlayersPoints;
};
//typedef TMap<APlayerController_Racing*, int8> Pointsmap;
DECLARE_MULTICAST_DELEGATE_TwoParams
	(FDelegateType_PointsUpdated, APlayerController_Racing*, int8);

UCLASS()
class LEARNING_CPP_RACING_API AGameState_Playing : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UUserWidget* ScreenWidget;
	APlayerController_Racing* PlayerController;
	virtual void BeginPlay()override;
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	FDelegateType_PointsUpdated DelegateList_UpdatePoints;
	UPROPERTY(EditDefaultsOnly)TSubclassOf<UUserWidget> ScreenWidgetClass;
		//=class UUserWidget_ScreenData::StaticClass();
	UUserWidget* GetScreenWidget();
	void IncreasePointsOfPayer(APlayerController_Racing* Controller,int8 Amount=1);
	UFUNCTION(NetMultiCast, Reliable)void Multi_IncreasePointsOfPlayer(APlayerController_Racing* Controller, int8 Amount = 1);
	void Multi_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount);
	UFUNCTION(Server,Reliable,WithValidation)void Server_IncreasePointsOfPlayer(APlayerController_Racing* Controller, int8 Amount = 1);
	void Server_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount );
	bool Server_IncreasePointsOfPlayer_Validate(APlayerController_Racing* Controller, int8 Amount );
	//TMap<APlayerController_Racing*, int8> PointsOfPlayers;
	UPROPERTY (ReplicatedUsing= OnRep_PointsChanged) FJustPointsMap Map_PlayersPoints;
	UFUNCTION()void OnRep_PointsChanged();
	UFUNCTION()void ChangePlayersPoints(APlayerController_Racing* Controller,int8 Diference);
};