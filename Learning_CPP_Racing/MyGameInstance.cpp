// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"


void UMyGameInstance::Server_SomeTestFunction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Instance: test server function called."));
	Multi_SomeTestFunction();
	//GetWorld()->GetAuthGameMode();
}

void UMyGameInstance::Multi_SomeTestFunction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Instance: test net multi cast function called."));
}

void UMyGameInstance::Init()
{
	Super::Init();
	if (IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = OnlineSubSystem->GetSessionInterface();
	}
	if (SessionInterface) {
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UMyGameInstance::SessionCreationFinished);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UMyGameInstance::JoinOnlineSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UMyGameInstance::SearchForSessionsComplete);
	}
}

bool UMyGameInstance::CreateOnlineSession(FName SessionName, FName HostName)
{
	if (SessionInCreation)return false;
	UE_LOG(LogTemp,Warning,TEXT("Game Instance: Creating session ..."));
	if (SessionName == "")SessionName = StandartSessionName;
	if (HostName == "")HostName = StandartHostName;
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUsesStats = true;
	SessionSettings.NumPrivateConnections = 5;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.Set(FName("SERVER_NAME_KEY"),SessionName.ToString(),EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName.ToString(),EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (SessionInterface)SessionInCreation = SessionInterface->CreateSession(0,SessionName, SessionSettings);

	return SessionInCreation;
}

void UMyGameInstance::SessionCreationFinished(FName SessionName, bool Result)
{
	SessionInCreation = false;
	if (Result == false)return;
	UE_LOG(LogTemp,Warning,TEXT("Game Instance: SessionCreation was successfull %s."),
		*SessionName.ToString());
	GetWorld()->ServerTravel("/Game/VehicleExampleMap?Listen");
}

bool UMyGameInstance::SearchForSessions()
{
	if (SessionsSearchInProgress || !SessionInterface.IsValid())return false;
	SessionsSearchInProgress = true;
	SessionSearchResult = MakeShareable(new FOnlineSessionSearch());
	SessionSearchResult->bIsLanQuery = true;
	SessionSearchResult->MaxSearchResults = 10000;
	SessionSearchResult->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0,SessionSearchResult.ToSharedRef());
	return true;
}

void UMyGameInstance::SearchForSessionsComplete(bool Success)
{
	SessionsSearchInProgress = false;
}

bool UMyGameInstance::JoinOnlineSession(FName SessionName, FOnlineSessionSearchResult& DesiredSession)
{
	if (!SessionInterface.IsValid()){UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining - no session inteface.")); return false;}
	if (SessionInCreation) { UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining.SessionInCreation.")); return false; }
	if (ConectingToSessionInProcess) { UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining.ConectingToSessionInProcess.")); return false; }
	if (SessionsSearchInProgress) { UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining.SessionsSearchInProgress.")); return false; }

	ConectingToSessionInProcess = SessionInterface->JoinSession(1, SessionName, DesiredSession);
	if (ConectingToSessionInProcess) { UE_LOG(LogTemp, Warning, TEXT("Game Instance: start joining session.")); }
	else  UE_LOG(LogTemp, Warning, TEXT("Game Instance: start joining session failed."));
	
	return ConectingToSessionInProcess;
}

void UMyGameInstance::JoinOnlineSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{
	UE_LOG(LogTemp, Error, TEXT("Game Instance: calling server test function."));
	Server_SomeTestFunction();
	ConectingToSessionInProcess = false;
	if (ResultType != EOnJoinSessionCompleteResult::AlreadyInSession &&
		ResultType != EOnJoinSessionCompleteResult::Success) {
		UE_LOG(LogTemp,Warning,TEXT("Game Instance: failed to join session."));
		return;
	}
	APlayerController* TempPController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(TempPController)) {UE_LOG(LogTemp,Warning,TEXT("GameInstance: failed to travel to new level, no controller was found."))};
	FString TravelAdress = "";
	if (!SessionInterface.IsValid()) { 
		UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to join session - no sessions interface.")); 
		return;
	};
	SessionInterface->GetResolvedConnectString(SessionName, TravelAdress);
	if (TravelAdress == "") {
		UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to join session - no adress was resolved."));
		return;
	}
	TempPController->ClientTravel(TravelAdress,ETravelType::TRAVEL_Absolute);
}