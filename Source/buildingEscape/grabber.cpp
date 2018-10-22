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

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(RayCastV, RayCast);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	auto LineTraceEnd = RayCastV + RayCast.Vector() * ScalarReach;


	GetWorld()->LineTraceSingleByObjectType(Hit, RayCastV, LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

void Ugrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	auto HitResult = GetFirstPhysicsBodyInReach();

	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}

}

void Ugrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Released"))

	PhysicsHandle->ReleaseComponent();
}

void Ugrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics Handle Found
	}
	else
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
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(RayCastV, RayCast);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	auto LineTraceEnd = RayCastV + RayCast.Vector() * ScalarReach;


	GetWorld()->LineTraceSingleByObjectType(Hit, RayCastV, LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	AActor* ActorHit = Hit.GetActor();
	return Hit;
}
