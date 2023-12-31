// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_ScreenData.h"
#include "UserWidget_PlayersPoints.h"
#include "MyGameInstance.h"
#include "PlayerController_Racing.h"
#include "PlayerState_Racing.h"
#include "Kismet/GameplayStatics.h"


bool UUserWidget_ScreenData::Initialize()
{
	Super::Initialize();
	if (Button_Resume)Button_Resume->OnClicked.AddDynamic(this, &UUserWidget_ScreenData::Resume);
	if (Button_Exit)Button_Exit->OnClicked.AddDynamic(this, &UUserWidget_ScreenData::QuitGame);
	if (Button_BackToMenu)Button_BackToMenu->OnClicked.AddDynamic(this, &UUserWidget_ScreenData::BackToMenu);
	PlayerController = Cast<APlayerController_Racing>(GetWorld()->GetFirstPlayerController());
	//if (PlayerController)SetPlayerState(PlayerController->GetPlayerState<APlayerState_Racing>());
	TryStartSettings();
	//GetGameState();
		

	/*
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle,this,
		&UUserWidget_ScreenData::RefreshPointsFromGameState,3);*/
	//bInitialized = true;
	//if(PlayerState)PlayerState->ChangePoints(0);
	return true;
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
	if (!GameState || !VerticalBox_Points)return;
	if(GEngine)GEngine->AddOnScreenDebugMessage(0,32,FColor::Red,
		FString("Screen Widget: refreshind points list from state."));
	//GameState->DelegateList_AllPointsUpdated.Remove(Handle_PointsUpdate);
	/*UE_LOG(LogTemp, Warning, TEXT("Screen widget: clearing poitns list. %i childs."),
		VerticalBox_Points->GetChildrenCount());*/
	VerticalBox_Points->ClearChildren();
	/*
	UE_LOG(LogTemp, Warning, TEXT("Screen widget: refreshing points list. players registred in game state %i."),
		GameState->GetSavedPoints()->PlayersPoints.Num());*/
	
	UUserWidget_PlayersPoints* TempPoints;
	for (FJustPointsMap Pair : PointsList) {
		//UE_LOG(LogTemp, Warning, TEXT("Screen widget: found saved points %i - %i."),	Pair.PlayerID, Pair.PlayersPoints);
		TempPoints = CreateWidget<UUserWidget_PlayersPoints>(PlayerController, PointsWidgetClass);
		TempPoints->InitializePointsWidget(Pair.PlayerID, Pair.PlayersPoints);
		VerticalBox_Points->AddChild(TempPoints);
	}
	//if(PlayerState)PlayerState->ChangePoints(0);
	
}

void UUserWidget_ScreenData::SetPlayerState(APlayerState_Racing* State)
{
	if (!IsValid(State)) {
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: passed empty player state."));
		return;
	}
	if (PlayerState) { 
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: already have player state."));
		return;
	}
	PlayerState = State; 
	PlayerState->ChangePoints(0);

	/*
	PlayerState->DelegateList_UpdatedPoints.AddUObject
	(this, &UUserWidget_ScreenData::UpdatePlayerPoints);
	PlayerState->ChangePoints(0);*/
}

UMyGameInstance* UUserWidget_ScreenData::GetGameInstance()
{
	if (GameInstance)return GameInstance;
	GameInstance = Cast< UMyGameInstance >(UGameplayStatics::GetGameInstance(GetWorld()));
	return GameInstance;
}

AGameState_Playing* UUserWidget_ScreenData::GetGameState()
{
	if (IsValid(GameState))return GameState;
	GameState = Cast<AGameState_Playing>(GetWorld()->GetGameState());
	if (IsValid(GameState)) {
		TArray<FJustPointsMap> PointsTemp= *GameState->GetSavedPoints();
		RefreshPointsFromGameState(PointsTemp);
		GameState->DelegateList_AllPointsUpdated.AddUObject
			(this, &UUserWidget_ScreenData::RefreshPointsFromGameState);
		GameState->DelegateList_PlayersPointsUpdated.AddUObject
			(this, &UUserWidget_ScreenData::UpdatePlayerPoints);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: cant attach self to gamestate points update."));
	}
	return GameState;
}

void UUserWidget_ScreenData::TryStartSettings()
{
	GetGameState();
	if (!IsValid(GameState)) {
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: loop process..."));
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this,
			&UUserWidget_ScreenData::TryStartSettings, 1);
	};
}

void UUserWidget_ScreenData::BackToMenu()
{
	bool TempBool{ false };
	if (GetGameInstance())TempBool = GameInstance->CurentlyInSession();
	if (!TempBool) UGameplayStatics::OpenLevel(GetWorld(),FName("Level_MainMenu"));
	GameInstance->LeaveSession();
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_MainMenu"));
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

void UUserWidget_ScreenData::UpdatePlayerPoints(int32 PlayerID,int8 NewValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Screen widget: update points delegat was trigered."));
	if (!IsValid(VerticalBox_Points)){
		UE_LOG(LogTemp, Warning, TEXT("Screen widget: no widget points box found."));
		return;
	}
	/*
	UE_LOG(LogTemp, Warning, TEXT("Screen widget: updating poitns list. %i childs."),
		VerticalBox_Points->GetChildrenCount());*/
	UUserWidget_PlayersPoints* CastedChild;
	bool bFoundWidget=false;
	for (UWidget* Child : VerticalBox_Points->GetAllChildren()) {
		CastedChild = Cast<UUserWidget_PlayersPoints>(Child);
		if (!IsValid(CastedChild))continue;
		if (CastedChild->PlayerID != PlayerID)continue;
		bFoundWidget = true;
		CastedChild->UpdateText(NewValue);
		break;
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
