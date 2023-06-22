// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
	Super::Init();
	if (IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = OnlineSubSystem->GetSessionInterface();
	}
	if (SessionInterface) {
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UMyGameInstance::SessionCreationFinished);
	}
}

bool UMyGameInstance::CreateOnlineSession(FName SessionName, FName HostName)
{
	if (SessionInCreation)return false;
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

	if (SessionInterface)SessionInCreation = SessionInterface->CreateSession(1,SessionName, SessionSettings);

	return SessionInCreation;
}

void UMyGameInstance::SessionCreationFinished(FName SessionName, bool Result)
{
	SessionInCreation = false;
	if (Result == false)return;
	UE_LOG(LogTemp,Warning,TEXT("GameInstance: SessionCreation was successfull.") );
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
	SessionInterface->FindSessions(1,SessionSearchResult.ToSharedRef());
	return true;
}

bool UMyGameInstance::SearchForSessionsComplete()
{
	SessionsSearchInProgress = false;
	return false;
}
