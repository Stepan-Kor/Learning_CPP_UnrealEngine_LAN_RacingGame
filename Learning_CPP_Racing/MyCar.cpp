// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCar.h"
#include "GameState_Playing.h"
#include "PlayerController_Racing.h"
#include "Learning_CPP_RacingWheelFront.h"
#include "Learning_CPP_RacingWheelRear.h"
#include "MyGameInstance.h"
#include "WheeledVehicleMovementComponent4w.h"
#include "PlayerState_Racing.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCar::AMyCar()
{
	///hggcfvg
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	/*
	FString TempString = IsValid(Vehicle4W) ? "Car constructor: Cast vehcl move comp valid." :
		"Car constructor: Cast vehcl move comp Not valid.";
	GEngine->AddOnScreenDebugMessage(1, 7, FColor::MakeRandomColor(), TempString);*/
	if(IsValid(Vehicle4W)){
	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = ULearning_CPP_RacingWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = ULearning_CPP_RacingWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = ULearning_CPP_RacingWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = ULearning_CPP_RacingWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);
	}
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Body Of Car");
	CameraArm=CreateDefaultSubobject<USpringArmComponent>("Back Camera Arm");
	CameraArm->SetupAttachment(GetMesh());
	CameraArm->TargetOffset = FVector(0.f,0,200);
	CameraArm->SetRelativeRotation(FRotator(-15,0,0));
	CameraArm->TargetArmLength = 600;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraRotationLagSpeed = 8;
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritRoll = false;
	BackCamera=CreateDefaultSubobject<UCameraComponent>("Back Camera");
	BackCamera->SetupAttachment(CameraArm);
	BackCamera->bUsePawnControlRotation = false;
	FrontCamera= CreateDefaultSubobject< UCameraComponent>("Front Camera");
	FrontCamera->SetupAttachment(GetMesh());
	SpeedPanelText= CreateDefaultSubobject<UTextRenderComponent>("Panel Text");
	SpeedPanelText->SetupAttachment(GetMesh());
	//comment21
	//bInReverseGear = false;
}


// Called when the game starts or when spawned
void AMyCar::BeginPlay()
{
	//UE_LOG(LogTemp, Error, TEXT("Pawn: START."));
	Super::BeginPlay();
	if (HasLocalNetOwner()) {
		GameState = Cast<AGameState_Playing >(UGameplayStatics::GetGameState(GetWorld()));
		RacingController = Cast< APlayerController_Racing>(GetController());
		//if(RacingController)ScreenWidget=RacingController->GetWidgetScreenData();
		//UE_LOG(LogTemp, Warning, TEXT("Car: begin play change points %d."), ChangePoints(0));
	}
}

void AMyCar::Pause()
{
	if (!IsValid(ScreenWidget))return;
	ScreenWidget->Pause();
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
	PlayerInputComponent->BindAction("Pause",EInputEvent::IE_Pressed,this, &AMyCar::Pause);
	PlayerInputComponent->BindAxis("MoveForward",this,&AMyCar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AMyCar::MoveRight);
}
APlayerState_Racing* AMyCar::GetPlayerStateRacing()
{
	if (PlayerState)return PlayerState;
	PlayerState=GetPlayerState<APlayerState_Racing>();
	return PlayerState;
}
void AMyCar::MoveForward(float Value)
{
	//if (Value == 0)return;
	//GEngine->AddOnScreenDebugMessage(1,0.2,FColor::Red,FString("Car: MoveForward input taken."));
	GetVehicleMovementComponent()->SetThrottleInput(Value);
}
void AMyCar::MoveRight(float Value)
{
	//if (Value == 0)return;
	//GEngine->AddOnScreenDebugMessage(1, 0.2, FColor::Red, FString("Car: MoveRight input taken."));
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

bool AMyCar::ChangePoints(int8 Diference)
{
	if (!HasLocalNetOwner())return false;
	//Cast<UMyGameInstance>(GetGameInstance())->Server_SomeTestFunction();
	if (GetPlayerStateRacing())PlayerState->ChangePoints(Diference);
	else {
		UE_LOG(LogTemp, Warning, TEXT("Car: cant get player state."));
		return false; }
	return true;
}

void AMyCar::Server_ChangePoints_Implementation(APlayerController_Racing* PlayerController, int8 Diference)
{
	Multi_ChangePoints(PlayerController, Diference);
}

void AMyCar::Multi_ChangePoints_Implementation(APlayerController_Racing* PlayerController, int8 Diference)
{
	if (!IsValid(GameState)){
		UE_LOG(LogTemp, Warning, TEXT("Car: Cant call game state - empty ponter."));
		return;
	}
	//(LogTemp, Warning, TEXT("Car: NetMulticast changing points."));
	//GameState->ChangePlayersPoints(PlayerController, Diference);
	
}
