// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


bool UMyGameInstance::TimeSinceJoinCreationPassed()
{
	float TimePased = UGameplayStatics::GetTimeSeconds(GetWorld()) - SessionCreationJoinStart;
	if (TimePased < 0) { 
		SessionCreationJoinStart = UGameplayStatics::GetTimeSeconds(GetWorld());
		TimePased = 0;
	}
	UE_LOG(LogTemp, Warning, TEXT("Game Instance: since join/creation time passed: %f"),		TimePased);
	return TimePased > 8;
}

bool UMyGameInstance::CurentlyInSession()
{
	if (!SessionInterface)return false;
	if (bCurrentlyInSession)return true;
	TArray <AActor*> ActorsTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerController::StaticClass(),ActorsTemp);
	for (auto PC : ActorsTemp) {
		if (!Cast<APlayerController>(PC)->IsLocalController()) {
			bCurrentlyInSession = true;
			break;
		};
	}
	return bCurrentlyInSession;
}

void UMyGameInstance::LeaveSession()
{
	bCurrentlyInSession = false;
	ConectingToSessionInProcess = false;
	if (!SessionInterface)return;
	SessionInterface->DestroySession(CurrentSessionName);
}

void UMyGameInstance::Server_SomeTestFunction_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Game Instance: test server function called."));
	Multi_SomeTestFunction();
	//GetWorld()->GetAuthGameMode();
}

void UMyGameInstance::Multi_SomeTestFunction_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Game Instance: test net multi cast function called."));
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

	SessionCreationJoinStart = UGameplayStatics::GetTimeSeconds(GetWorld());
	return SessionInCreation;
}

void UMyGameInstance::SessionCreationFinished(FName SessionName, bool Result)
{
	SessionInCreation = false;
	if (Result == false)return;
	/*UE_LOG(LogTemp,Warning,TEXT("Game Instance: SessionCreation was successfull %s."),
		*SessionName.ToString());*/
	CurrentSessionName = SessionName;
	bCurrentlyInSession = true;
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
	if (SessionInCreation) { 
		if (TimeSinceJoinCreationPassed()) {
			UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining.SessionInCreation. error: %i. time: %f"),
				SessionInterface->GetSessionState(SessionName));
			return false; 
		}
		SessionInCreation = false;
	}
	if (ConectingToSessionInProcess) { 
		if (TimeSinceJoinCreationPassed()) {
			UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining.ConectingToSessionInProcess. error: %i. time: %f"),
				SessionInterface->GetSessionState(SessionName));
			return false; 
		}
		ConectingToSessionInProcess = false;
	}
	if (CurentlyInSession())LeaveSession();
	//if (SessionsSearchInProgress) { UE_LOG(LogTemp, Warning, TEXT("Game Instance: failed to start joining.SessionsSearchInProgress.")); return false; }
	CurrentSessionName = SessionName;
	SessionCreationJoinStart=UGameplayStatics::GetTimeSeconds(GetWorld());
	ConectingToSessionInProcess = SessionInterface->JoinSession(1, SessionName, DesiredSession);/*
	if (ConectingToSessionInProcess) { UE_LOG(LogTemp, Warning, TEXT("Game Instance: start joining session.")); }
	else  UE_LOG(LogTemp, Warning, TEXT("Game Instance: start joining session failed."));
	*/
	return ConectingToSessionInProcess;
}

void UMyGameInstance::JoinOnlineSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{
	//UE_LOG(LogTemp, Error, TEXT("Game Instance: calling server test function."));
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
	CurrentSessionName = SessionName;
	bCurrentlyInSession = true;
	TempPController->ClientTravel(TravelAdress,ETravelType::TRAVEL_Absolute);
}