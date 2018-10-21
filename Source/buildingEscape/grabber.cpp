// Fill out your copyright notice in the Description page of Project Settings.

#include "grabber.h"


// Sets default values for this component's properties
Ugrabber::Ugrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void Ugrabber::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void Ugrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(RayCastV, RayCast);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	auto LineTraceEnd = RayCastV + RayCast.Vector() * ScalarReach;


	GetWorld()->LineTraceSingleByObjectType(Hit, RayCastV, LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("hit trace: %s"), *(ActorHit->GetName()))
	}
}

