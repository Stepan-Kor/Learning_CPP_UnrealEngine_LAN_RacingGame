// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SessionDesscription.h"

void UWidget_SessionDesscription::StartSetting(FText SessionName, FText HostName, FText PlaersAmount)
{
	TextBlock_HostName->SetText(HostName);
	TextBlock_SessionName->SetText(SessionName);
	TextBlock_PlayersAmount->SetText(PlaersAmount);
}
