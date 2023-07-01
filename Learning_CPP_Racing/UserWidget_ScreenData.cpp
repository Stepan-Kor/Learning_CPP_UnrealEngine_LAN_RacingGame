// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_ScreenData.h"
#include "UserWidget_PlayersPoints.h"
#include "PlayerController_Racing.h"
#include "PlayerState_Racing.h"
#include "Kismet/GameplayStatics.h"


bool UUserWidget_ScreenData::Initialize()
{
	Super::Initialize();
	if (Button_Resume)Button_Resume->OnClicked.AddDynamic(this, &UUserWidget_ScreenData::Resume);
	if (Button_Exit)Button_Exit->OnClicked.AddDynamic(this, &UUserWidget_ScreenData::QuitGame);
	PlayerController = Cast<APlayerController_Racing>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)PlayerState = PlayerController->GetPlayerState<APlayerState_Racing>();

	if (PlayerState)PlayerState->DelegateList_UpdatedPoints.AddUObject
	(this, &UUserWidget_ScreenData::PlayerStatePointsUpdated); 
	else UE_LOG(LogTemp, Warning, TEXT("ScreenWidget: Cant get player state."));

	GameState = Cast<AGameState_Playing>(GetWorld()->GetGameState());
	if (GameState &&false) {
		if (GameState->bPointsWasReplicated) { 
			RefreshPointsFromGameState(*GameState->GetSavedPoints());
		}
		else
			Handle_PointsUpdate = GameState->DelegateList_UpdatePoints.AddUObject
		(this, &UUserWidget_ScreenData::RefreshPointsFromGameState);/*
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: players registred in game state %i."),
			GameState->GetSavedPoints()->PlayersPoints.Num());*/
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: cant attach self to gamestate points update."));
	}
	/*
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle,this,
		&UUserWidget_ScreenData::RefreshPointsFromGameState,3);*/
	return true;
}

void UUserWidget_ScreenData::PlayerStatePointsUpdated(int8 NewValue)
{
	if (!PlayerState)return;
	UpdatePointsVisualization(PlayerState->GetPlayerId(),NewValue);

}
void UUserWidget_ScreenData::Resume()
{
	if (!IsValid(PlayerController))return;
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(false);
	VerticalBox_Buttons->SetVisibility(ESlateVisibility::Hidden);
}

void UUserWidget_ScreenData::RefreshPointsFromGameState(const TArray<FJustPointsMap>& PointsList)
{

	return;
	if (!GameState || !VerticalBox_Buttons)return;
	if (GameState)GameState->DelegateList_UpdatePoints.Remove(Handle_PointsUpdate);
	VerticalBox_Buttons->ClearChildren();/*
	UE_LOG(LogTemp, Warning, TEXT("Screen widget: refreshing points list. players registred in game state %i."),
		GameState->GetSavedPoints()->PlayersPoints.Num());*/
	
	UUserWidget_PlayersPoints* TempPoints;
	for (FJustPointsMap Pair : PointsList) {

		UE_LOG(LogTemp, Warning, TEXT("Screen widget: found saved points %i - %i."),
			Pair.PlayerID, Pair.PlayersPoints);
		TempPoints = CreateWidget<UUserWidget_PlayersPoints>(PlayerController, PointsWidgetClass);
		TempPoints->InitializePointsWidget(Pair.PlayerID, Pair.PlayersPoints);
		VerticalBox_Points->AddChild(TempPoints);
	}
	
}

void UUserWidget_ScreenData::SetPlayerState(APlayerState_Racing* State)
{
	if (PlayerState)return;
	PlayerState = State; 
	if(PlayerState)PlayerState->DelegateList_UpdatedPoints.AddUObject
	(this, &UUserWidget_ScreenData::PlayerStatePointsUpdated);
}

void UUserWidget_ScreenData::BackToMenu()
{

}

void UUserWidget_ScreenData::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,true);
}

void UUserWidget_ScreenData::Pause()
{
	if (!IsValid(PlayerController))return;
	VerticalBox_Buttons->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}

void UUserWidget_ScreenData::UpdatePointsVisualization(int32 PlayerID,int8 NewValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Screen widget: update points delegat was trigered."));
	if (!IsValid(VerticalBox_Points)){
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: no widget points box found."));
		return;
	}
	UUserWidget_PlayersPoints* CastedChild;
	bool bFoundWidget=false;
	for (UWidget* Child : VerticalBox_Points->GetAllChildren()) {
		CastedChild = Cast<UUserWidget_PlayersPoints>(Child);
		if (!IsValid(CastedChild))continue;
		if (CastedChild->PlayerID != PlayerID)continue;
		bFoundWidget = true;
		CastedChild->UpdateText(NewValue);
	};
	if (bFoundWidget)return;
	if (!IsValid(PointsWidgetClass)) {
		UE_LOG(LogTemp,Warning,TEXT("Screen Widget: no class for points visualization."));
		return;
	}
	UUserWidget_PlayersPoints* TempPoints =
		CreateWidget<UUserWidget_PlayersPoints>(PlayerController, PointsWidgetClass);
	TempPoints->InitializePointsWidget(PlayerID, NewValue);
	VerticalBox_Points->AddChild(TempPoints);
}
