// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WheeledVehicle.h"
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
	void ChangePoints(int8 Diference);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Pause();
	class UUserWidget_ScreenData* ScreenWidget;
	class AGameState_Playing* GameState;
	void MoveForward(float Value);
	void MoveRight(float Value);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
