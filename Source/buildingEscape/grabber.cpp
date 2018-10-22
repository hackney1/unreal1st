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

	FindPhysicsHandle();
	SetupInputComponent();

}


// Called every frame
void Ugrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

void Ugrabber::Grab()
{
	auto HitResult = GetFirstPhysicsBodyInReach();

	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (!PhysicsHandle) { return; }

	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}

}

void Ugrabber::Released()
{
	if (!PhysicsHandle) { return; }

	PhysicsHandle->ReleaseComponent();
}

void Ugrabber::FindPhysicsHandle()
{
	if (!PhysicsHandle) { return; }

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: PhysicsHandle of %S is missing"), *GetOwner()->GetName())
	}
}

void Ugrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Iput Component found!"))
		InputComponent->BindAction("Grab", IE_Pressed, this, &Ugrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &Ugrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: InputComponent of %S is missing"), *GetOwner()->GetName())
	}
}

const FHitResult Ugrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(Hit, GetReachLineStart(), GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	AActor* ActorHit = Hit.GetActor();
	return Hit;
}
FVector Ugrabber::GetReachLineStart()
{
	 GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(RayCastV, RayCast);

	 return RayCastV;
}

FVector	Ugrabber::GetReachLineEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(RayCastV, RayCast);

	auto LineTraceEnd = RayCastV + RayCast.Vector() * ScalarReach;

	return LineTraceEnd;

}