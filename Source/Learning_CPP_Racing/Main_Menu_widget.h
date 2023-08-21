// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_SessionDesscription.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "MyGameInstance.h"
#include "Main_Menu_widget.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API UMain_Menu_widget : public UUserWidget
{
	GENERATED_BODY()
public:
		virtual bool Initialize()override;
		UFUNCTION()void SessionCreated(FName SessionName,bool Result);
		UFUNCTION()void SessionsListUpdated(bool Result);
		UFUNCTION()void UpdateSessionsList();
protected:
	IOnlineSessionPtr OnlineInterface;
	APlayerController* PlayerController;
	class AGameModeBase* GameMode;
	UFUNCTION() void CreateSession();
	UMyGameInstance* GameInstance;
	FName NewSessionName = "Standart Session Name";
	UPROPERTY(meta = (BindWidget))UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget))UButton* Button_CreateSession;
	UPROPERTY(meta = (BindWidget))UButton* Button_SoloGame;
	UPROPERTY(meta = (BindWidget))UButton* Button_RefreshSessionList;
	UPROPERTY(meta = (BindWidget))UScrollBox* ScrollBox_SessionsList;
	UPROPERTY(meta = (BindWidget))UEditableTextBox* TextBox_SessionName;
	UFUNCTION() void QuitGame();
	UFUNCTION() void StartSoloGame();
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UWidget_SessionDesscription> WidgetSessionDescription =
		UWidget_SessionDesscription::StaticClass();
	
};
