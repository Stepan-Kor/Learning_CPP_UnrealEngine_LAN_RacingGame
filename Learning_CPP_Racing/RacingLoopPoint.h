// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RacingLoopPoint.generated.h"

UCLASS()
class LEARNING_CPP_RACING_API ARacingLoopPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARacingLoopPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)UStaticMeshComponent* Mesh;
	UFUNCTION()void BodyStartOverap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& HitResut);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)ARacingLoopPoint* NextPoint;
	UPROPERTY(EditAnywhere)bool bFinishPoint=false;

};
