// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	
}

void UOpenDoor::OpenDoor()
{
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto CurrentTick = GetWorld()->GetTimeSeconds();

	if (GetMassOnPlate() > MassLimit )
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (LastDoorOpenTime + DoorCloseDelay < CurrentTick)
	{
		CloseDoor();
	}
}
float UOpenDoor::GetMassOnPlate()
{
	TArray<AActor*> OverLappingActors;
	float MassOnPlate = 0.0f;

	if (!PressurePlate) { return MassOnPlate; }

	PressurePlate->GetOverlappingActors(OverLappingActors);

	for (auto* Actor : OverLappingActors)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *Actor->GetName())
		MassOnPlate += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return MassOnPlate;
}


