// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Main_Menu_widget.h"
#include "GameModeBase_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API AGameModeBase_MainMenu : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> WidgetClass_MainMenu = UMain_Menu_widget::StaticClass();
protected:
	virtual void BeginPlay() override;
	UUserWidget* Widget_MainMenu;
};
