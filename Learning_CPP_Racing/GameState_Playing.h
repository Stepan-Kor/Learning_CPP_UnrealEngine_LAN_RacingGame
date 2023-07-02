// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerController_Racing.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "GameState_Playing.generated.h"

/**
 */
USTRUCT() struct FJustPointsMap {
	GENERATED_USTRUCT_BODY()
	FJustPointsMap(int32 ID, int8 Points) : PlayerID(ID),PlayersPoints(Points) {	};
	FJustPointsMap(int32 ID) : PlayerID(ID),PlayersPoints(0) {	};
	FJustPointsMap() : PlayerID(0),PlayersPoints(0) {	};
	UPROPERTY()int32 PlayerID;
	UPROPERTY()int8 PlayersPoints;
	//UPROPERTY()int8 UpdateForcer;
};
//typedef TMap<APlayerController_Racing*, int8> Pointsmap;
DECLARE_MULTICAST_DELEGATE_OneParam
	(FDelegateType_PointsUpdated, const TArray <FJustPointsMap>&);
DECLARE_MULTICAST_DELEGATE_TwoParams
	(FDelegateType_PlayersPointsUpdated, int32, int8);

UCLASS()
class LEARNING_CPP_RACING_API AGameState_Playing : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UUserWidget* ScreenWidget;
	APlayerController_Racing* PlayerController;
	virtual void BeginPlay()override;
	APlayerState_Racing* PlayerState;
	UPROPERTY(Transient, EditAnywhere,ReplicatedUsing= OnRep_PointsChanged)
		TArray <FJustPointsMap> Map_PlayersPoints;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_PointsChanged)
		FString ReplicatedString {"Server old string."};
	UPROPERTY(Transient, EditAnywhere, ReplicatedUsing = OnRep_PointsChanged)
		TArray<int32> ReplicatedTestInt {0,1,2,3};
	void PrintInfo();
	UFUNCTION(Server, Unreliable)void Server_TestCall();
	void Server_TestCall_Implementation();
public:
	bool GetPlayerStateRacing(APlayerController_Racing* LPlayerController=nullptr,
		APlayerState_Racing* LPlayerState=nullptr);
	bool bPointsWasReplicated{false};
	TArray<FJustPointsMap>* GetSavedPoints();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	FDelegateType_PointsUpdated DelegateList_UpdatePoints;
	FDelegateType_PlayersPointsUpdated DelegateList_PlayersPointsUpdated;
	UPROPERTY(EditDefaultsOnly)TSubclassOf<UUserWidget> ScreenWidgetClass;
	UFUNCTION(NetMultiCast, Reliable)void Multi_IncreasePointsOfPlayer(int32 PlayerID, int8 Amount = 0);
	void Multi_IncreasePointsOfPlayer_Implementation(int32 PlayerID, int8 Amount=0);
	UFUNCTION()void OnRep_PointsChanged();
	UFUNCTION()void ChangePlayersPoints(int32 PlayerStateID,int8 NewValue);
};