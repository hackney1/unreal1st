// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API Ugrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	Ugrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

private:

	FRotator RayCast;
	FVector RayCastV;
	float ScalarReach{ 100.f };
	FHitResult Hit;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	void Grab();

	void Released();

	void FindPhysicsHandle();

	void SetupInputComponent();

	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineStart();

	FVector GetReachLineEnd();
	

};
