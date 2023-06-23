// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "UserWidget_ScreenData.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API UUserWidget_ScreenData : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))UButton* Button_Resume;
	UPROPERTY(meta = (BindWidget))UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget))UButton* Button_BackToMenu;
	UPROPERTY(meta = (BindWidget))UVerticalBox* VerticalBox_Buttons;
	void Pause();
protected:
	virtual bool Initialize() override;
	UFUNCTION() void Resume();
	APlayerController* PlayerController;
	UFUNCTION() void BackToMenu();
	UFUNCTION() void QuitGame();
};
