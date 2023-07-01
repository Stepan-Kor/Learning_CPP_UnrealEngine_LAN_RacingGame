// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingLoopPoint.h"
#include "MyCar.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARacingLoopPoint::ARacingLoopPoint ()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ARacingLoopPoint::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh) {
		Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARacingLoopPoint::BodyStartOverap);
		if (!bFinishPoint)Mesh->SetVisibility(false);
	}
	GameState = Cast<AGameState_Playing >(UGameplayStatics::GetGameState(GetWorld()));
}

void ARacingLoopPoint::BodyStartOverap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& HitResut)
{
	if (!Mesh->IsVisible())return;
	//UE_LOG(LogTemp,Warning,TEXT("Loop Point: overlaped by %s."),*OtherActor->GetName());
	AMyCar* Car = Cast<AMyCar>(OtherActor);
	if (!IsValid(Car) || !IsValid(NextPoint)) return;
	//if(!Car->HasAuthority())return;

	if (!Car->ChangePoints(1))return;
	FString ControllerName = "no controller";
	AController* TempContr = Car->GetController();
	if (IsValid(TempContr)) { ControllerName = TempContr->GetName(); };
	/*UE_LOG(LogTemp,Warning,TEXT("Loop Point(%d): overlaped %s, has authority - %d, controller -%s."),
		HasAuthority(),*Car->GetName(),Car->HasAuthority(), *ControllerName);*/
	Mesh->SetVisibility(false);
	NextPoint->Mesh->SetVisibility(true);
	
}

// Called every frame
void ARacingLoopPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

