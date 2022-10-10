// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UPhysicsHandleComponent *PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("GOT PHYSICS HANDLE: %s"), *PhysicsHandle->GetName());
		PhysicsHandle->GetName();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PHYSICS HANDLE FOUND"));
	}

	// ... Using the physics handle property to grab and release our actors
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent *PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle==nullptr)
	{
		return;
	}
	FVector TargetLocation = GetComponentLocation() + GetForwardVector()*holdDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation,GetComponentRotation());

	// ...
}
void UGrabber::Grab()
{
	UPhysicsHandleComponent *PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle==nullptr)
	{
		return;
	}
	
	FVector Start =  GetComponentLocation();// GetOwner()->GetActorLocation(); ////
	
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	
	
	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	FHitResult hitResult;
	bool hasHit = GetWorld()->SweepSingleByChannel(
		hitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere);
		DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 10,10,FColor::Yellow,false,5);
		DrawDebugSphere(GetWorld(), hitResult.Location, 10,10,FColor::Blue,false,5);

	if (hasHit)
	{
		UPrimitiveComponent* HitComponent = hitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			hitResult.GetComponent(),NAME_None,hitResult.ImpactPoint,GetComponentRotation());
			//UE_LOG(LogTemp, Display, TEXT("GRABBED: %f"),hitResult.GetActor()->GetActorNameOrLabel());
	}
}

void UGrabber::Release()
{
	
}
