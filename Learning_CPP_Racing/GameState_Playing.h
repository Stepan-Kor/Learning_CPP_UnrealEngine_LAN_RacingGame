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
	TMap<int32, int8> PlayersPoints;
};
//typedef TMap<APlayerController_Racing*, int8> Pointsmap;
DECLARE_MULTICAST_DELEGATE_TwoParams
	(FDelegateType_PointsUpdated, int32, int8);

UCLASS()
class LEARNING_CPP_RACING_API AGameState_Playing : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UUserWidget* ScreenWidget;
	APlayerController_Racing* PlayerController;
	virtual void BeginPlay()override;
	UPROPERTY(Replicated) FJustPointsMap Map_PlayersPoints {};
	void PrintInfo();
public:
	FJustPointsMap* GetSavedPoints();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	FDelegateType_PointsUpdated DelegateList_UpdatePoints;
	UPROPERTY(EditDefaultsOnly)TSubclassOf<UUserWidget> ScreenWidgetClass;
	UUserWidget* GetScreenWidget();
	UFUNCTION(NetMultiCast, Reliable)void Multi_IncreasePointsOfPlayer(APlayerController_Racing* Controller, int8 Amount = 0);
	void Multi_IncreasePointsOfPlayer_Implementation(APlayerController_Racing* Controller, int8 Amount=0);
	UFUNCTION(Server,Reliable,WithValidation)void Server_IncreasePointsOfPlayer(int32 PlayerStateID, int8 Amount = 0);
	void Server_IncreasePointsOfPlayer_Implementation(int32 PlayerStateID, int8 Amount=0 );
	bool Server_IncreasePointsOfPlayer_Validate(int32 PlayerStateID, int8 Amount=0 );
	TMap<APlayerController_Racing*, int8> PointsOfPlayers;
	UFUNCTION()void OnRep_PointsChanged();
	UFUNCTION()void ChangePlayersPoints(int32 PlayerStateID,int8 NewValue);
};