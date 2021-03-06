// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

#define OUT

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

	// Find the owning Actor
	Owner = GetOwner();

	// OpenDoor();
	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate"), *GetOwner()->GetName())
	}
}


//void UOpenDoor::OpenDoor()
//{
//	// Set the door rotation
//	// Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));	// pitch, yaw, rolling
//	OnOpenRequest.Broadcast();
//}


//void UOpenDoor::CloseDoor()
//{
//	// Set the door rotation
//	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));	// pitch, yaw, rolling
//}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the ActorThatOpens is in the volume 
	// if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))

	// Poll the Trigger Volume 
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)		// TODO make into a parameter
	{
		OnOpen.Broadcast();
		//OpenDoor();
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		OnClose.Broadcast();		
	}

	// Check if it's time to close the door
	//if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) 
	//{
	//	CloseDoor();
	//}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {
		return 0.f;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// iterate through them adding their masses
	for (const auto *Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}
	return TotalMass;
}
