// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "Components/TextBlock.h"
#include "Widget_SessionDesscription.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING_CPP_RACING_API UWidget_SessionDesscription : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize()override;
	void StartSetting(FText SessionName,FText HostName,FText PlaersAmount, FOnlineSessionSearchResult& Session);
	UFUNCTION()void JoinSession();
protected:
	UPROPERTY(meta = (BindWidget)) UButton* Button_Conect;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_SessionName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_HostName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBlock_PlayersAmount;
	FName NameOfSession{};
	FOnlineSessionSearchResult SessionResult;
};
