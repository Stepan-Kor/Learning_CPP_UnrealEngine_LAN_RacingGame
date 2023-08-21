// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "Learning_CPP_RacingHud.generated.h"


UCLASS(config = Game)
class ALearning_CPP_RacingHud : public AHUD
{
	GENERATED_BODY()

public:
	ALearning_CPP_RacingHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
