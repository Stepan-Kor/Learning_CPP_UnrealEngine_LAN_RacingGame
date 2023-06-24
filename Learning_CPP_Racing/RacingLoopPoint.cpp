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
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARacingLoopPoint::BodyStartOverap);
	if (!bFinishPoint)Mesh->SetVisibility(false);
	GameState = Cast<AGameState_Playing >(UGameplayStatics::GetGameState(GetWorld()));
}

void ARacingLoopPoint::BodyStartOverap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& HitResut)
{
	if (!Mesh->IsVisible())return;
	UE_LOG(LogTemp,Warning,TEXT("Loop Point: overlaped by %s."),*OtherActor->GetName());
	AMyCar* Car = Cast<AMyCar>(OtherActor);
	if (IsValid(Car) && IsValid(NextPoint)) {
		if(!Car->HasAuthority())return;
		UE_LOG(LogTemp,Warning,TEXT("Loop Point: Owner of car - %d."),Car->HasAuthority());
		Mesh->SetVisibility(false);
		NextPoint->Mesh->SetVisibility(true);
		Car->ChangePoints(1);
	}
}

// Called every frame
void ARacingLoopPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

