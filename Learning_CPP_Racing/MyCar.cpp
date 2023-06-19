// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCar.h"

// Sets default values
AMyCar::AMyCar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Body Of Car");
	CameraArm=CreateDefaultSubobject<USpringArmComponent>("Back Camera Arm");
	BackCamera=CreateDefaultSubobject<UCameraComponent>("Back Camera");
	BackCamera->SetupAttachment(CameraArm);
	BackCamera->bUsePawnControlRotation = false;
	FrontCamera= CreateDefaultSubobject< UCameraComponent>("Front Camera");
	SpeedPanelText= CreateDefaultSubobject<UTextRenderComponent>("Panel Text");;
}

// Called when the game starts or when spawned
void AMyCar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

