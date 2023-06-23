// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SessionDesscription.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UWidget_SessionDesscription::Initialize() {
	Super::Initialize();
	if (Button_Conect)Button_Conect->OnClicked.AddDynamic(this,&UWidget_SessionDesscription::JoinSession);
	return true;
}
void UWidget_SessionDesscription::StartSetting(FText SessionName, FText HostName, FText PlaersAmount,
	FOnlineSessionSearchResult& Session)
{
	NameOfSession = FName( SessionName.ToString());
	SessionResult = Session;
	TextBlock_HostName->SetText(HostName);
	TextBlock_SessionName->SetText(SessionName);
	TextBlock_PlayersAmount->SetText(PlaersAmount);
}

void UWidget_SessionDesscription::JoinSession()
{
	UE_LOG(LogTemp,Warning,TEXT("Widget Session Description: join button pressed."));
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!IsValid(GameInstance))return;
	GameInstance->JoinOnlineSession(NameOfSession, SessionResult);
}
