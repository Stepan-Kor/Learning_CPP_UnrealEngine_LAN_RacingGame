// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBase_MainMenu.h"
#include "Kismet/GameplayStatics.h"

void AGameModeBase_MainMenu::BeginPlay()
{
	Super::BeginPlay();
	//UGameplayStatics::CreateWidget
	//GetGameState().PlayerController
	if(IsValid(WidgetClass_MainMenu))
		Widget_MainMenu=CreateWidget<UMain_Menu_widget>(GetWorld(), WidgetClass_MainMenu);
	if (Widget_MainMenu)Widget_MainMenu->AddToViewport();
	else UE_LOG(LogTemp,Warning,TEXT("GameMode: failed creating main menu wodget."));
}
