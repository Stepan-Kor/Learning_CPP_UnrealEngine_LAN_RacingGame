// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_Menu_widget.h"
#include "Gameframework/GameModeBase.h"
#include "OnlineSubSystem.h"
#include "Widget_SessionDesscription.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

bool UMain_Menu_widget::Initialize(){
	Super::Initialize();
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) { 
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
	if (Button_Exit)Button_Exit->OnClicked.AddDynamic(this,&UMain_Menu_widget::QuitGame);
	if (Button_SoloGame)Button_SoloGame->OnClicked.AddDynamic(this,&UMain_Menu_widget::StartSoloGame);
	if (Button_CreateSession)Button_CreateSession->OnClicked.AddDynamic(this,&UMain_Menu_widget::CreateSession);
	if (Button_RefreshSessionList)Button_RefreshSessionList->OnClicked.AddDynamic(this,&UMain_Menu_widget::UpdateSessionsList);

	GameMode = UGameplayStatics::GetGameMode(GetWorld());
	GameInstance = Cast< UMyGameInstance >(UGameplayStatics::GetGameInstance(GetWorld()));
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	
	if (OnlineSubsystem)OnlineInterface = OnlineSubsystem->GetSessionInterface();
	if (OnlineInterface) {
		//поключить оповещение что список сессий обновился
		OnlineInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UMain_Menu_widget::SessionsListUpdated);
		OnlineInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UMain_Menu_widget::SessionCreated);
	}
	return true;
}

void UMain_Menu_widget::SessionCreated(FName SessionName, bool Result)
{
	Button_CreateSession->SetIsEnabled(true);
}

void UMain_Menu_widget::SessionsListUpdated(bool Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Main menu widget: Update sessions list done %d."),Result);
	Button_RefreshSessionList->SetIsEnabled(true);
	UWidget_SessionDesscription* TempWidget; 
	ScrollBox_SessionsList->ClearChildren();
	FString SessionName,HostName;
	FString PlayersNumberString;
	int32  PlayersNumberInt32=0;
	for(FOnlineSessionSearchResult SomeSession : GameInstance->SessionSearchResult->SearchResults) {
		SessionName = "Uknown session";
		SomeSession.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), SessionName);
		UE_LOG(LogTemp, Warning, TEXT("MainMenu widget: found %s session."),SessionName);
		HostName = "Uknown host";
		SomeSession.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);
		PlayersNumberInt32= SomeSession.Session.SessionSettings.NumPrivateConnections
			+ SomeSession.Session.SessionSettings.NumPublicConnections;
		PlayersNumberString = FString::FromInt( PlayersNumberInt32);
		PlayersNumberString += "/5";
		TempWidget = CreateWidget<UWidget_SessionDesscription>(this, UWidget_SessionDesscription::StaticClass());
		TempWidget->StartSetting(FText::FromString(SessionName),
			FText::FromString(HostName), FText::FromString(PlayersNumberString));
		ScrollBox_SessionsList->AddChild(TempWidget);
	}
	
}

void UMain_Menu_widget::UpdateSessionsList()
{
	UE_LOG(LogTemp,Warning,TEXT("Main menu widget: Update sessions list pressed."));
	if (!IsValid(GameInstance))return;
	Button_RefreshSessionList->SetIsEnabled(false);
	GameInstance->SearchForSessions();

}

void UMain_Menu_widget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),PlayerController,EQuitPreference::Quit,true);
}

void UMain_Menu_widget::StartSoloGame()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName("VehicleExampleMap"));
}

void UMain_Menu_widget::CreateSession()
{
	bool StartCreating = false;
	if (TextBox_SessionName)NewSessionName = FName(TextBox_SessionName->GetText().ToString());
	if (IsValid(GameInstance))StartCreating=GameInstance->CreateOnlineSession(NewSessionName);
	if (StartCreating)Button_CreateSession->SetIsEnabled ( false);

}
