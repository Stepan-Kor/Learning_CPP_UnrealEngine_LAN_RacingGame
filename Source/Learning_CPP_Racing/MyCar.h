// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WheeledVehicle.h"
#include "UserWidget_ScreenData.h"
#include "Components/TextRenderComponent.h"
#include "MyCar.generated.h"

UCLASS()
class LEARNING_CPP_RACING_API AMyCar : public AWheeledVehicle //: public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyCar();
	//UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly) USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly) USpringArmComponent* CameraArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly) UCameraComponent* BackCamera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly) UCameraComponent* FrontCamera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly) UTextRenderComponent* SpeedPanelText;
	bool ChangePoints(int8 Diference=0);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UUserWidget_ScreenData* ScreenWidget;
	APlayerState_Racing* GetPlayerStateRacing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Pause();
	class AGameState_Playing* GameState;
	void MoveForward(float Value);
	void MoveRight(float Value);
	UFUNCTION(Server,Reliable) void Server_ChangePoints(APlayerController_Racing* PlayerController, int8 Diference);
	void Server_ChangePoints_Implementation(APlayerController_Racing* PlayerController, int8 Diference);
	UFUNCTION(NetMulticast,Reliable) void Multi_ChangePoints(APlayerController_Racing* PlayerController, int8 Diference);
	void Multi_ChangePoints_Implementation(APlayerController_Racing* PlayerController,int8 Diference);
	class APlayerController_Racing* RacingController;
	APlayerState_Racing* PlayerState;
};