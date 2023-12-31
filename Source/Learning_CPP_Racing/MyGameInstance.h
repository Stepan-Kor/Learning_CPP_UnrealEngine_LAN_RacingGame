// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubSystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	const FName StandartHostName{"Standart Host Name"};
	const FName StandartSessionName{"Standart Session Name"};
	bool SessionInCreation = false;
	bool SessionsSearchInProgress = false;
	bool ConectingToSessionInProcess = false;
	bool bCurrentlyInSession{ false };
	IOnlineSessionPtr SessionInterface;
	FName CurrentSessionName;
	float SessionCreationJoinStart;
public:
	bool TimeSinceJoinCreationPassed();
	bool CurentlyInSession();
	void LeaveSession();
	UFUNCTION(Server, Reliable)void Server_SomeTestFunction();
	void Server_SomeTestFunction_Implementation();
	UFUNCTION(NetMultiCast, Reliable)void Multi_SomeTestFunction();
	void Multi_SomeTestFunction_Implementation();
	virtual void Init()override;
	bool CreateOnlineSession(FName SessionName,FName HostName ="");
	UFUNCTION()void SessionCreationFinished(FName SessionName,bool Result);
	TSharedPtr <FOnlineSessionSearch> SessionSearchResult;
	bool SearchForSessions();
	UFUNCTION()void SearchForSessionsComplete(bool Success);
	bool JoinOnlineSession(FName SessionName,FOnlineSessionSearchResult& DesiredSession);
	void JoinOnlineSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type ResultType);
};