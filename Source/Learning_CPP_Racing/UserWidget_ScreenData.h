// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameState_Playing.h"
#include "Components/VerticalBox.h"
#include "UserWidget_ScreenData.generated.h"

/**
 * 
 */
struct FJustPointsMap;

UCLASS()
class LEARNING_CPP_RACING_API UUserWidget_ScreenData : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))UButton* Button_Resume;
	UPROPERTY(meta = (BindWidget))UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget))UButton* Button_BackToMenu;
	UPROPERTY(meta = (BindWidget))UVerticalBox* VerticalBox_Buttons;
	UPROPERTY(meta = (BindWidget))UVerticalBox* VerticalBox_Points;
	void Pause();
	UFUNCTION() void UpdatePlayerPoints(int32 PlayerID, int8 NewValue);
	UPROPERTY(EditDefaultsOnly)TSubclassOf<class UUserWidget_PlayersPoints> PointsWidgetClass;
	UFUNCTION() void RefreshPointsFromGameState(const TArray <FJustPointsMap>& PointsList);
	void SetPlayerState(APlayerState_Racing* State);
	class UMyGameInstance* GetGameInstance();
	class AGameState_Playing* GetGameState();
protected:
	void TryStartSettings();
	bool bInitialized{false};
	FDelegateHandle Handle_PointsUpdate;
	virtual bool Initialize() override;
	UFUNCTION() void Resume();
	class APlayerController_Racing* PlayerController;
	class APlayerState_Racing* PlayerState;
	class AGameState_Playing* GameState;
	class UMyGameInstance* GameInstance;
	UFUNCTION() void BackToMenu();
	UFUNCTION() void QuitGame();
};