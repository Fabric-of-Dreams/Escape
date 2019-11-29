// Copyright Sergey Miroshnichenko 2019


#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

#define OUT

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

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent found in %s"), *GetOwner()->GetName());
	}

	CheckInput();
}

void UGrabber::CheckInput()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// Bind the input axis
		//UE_LOG(LogTemp, Warning, TEXT("bGrabbed = %s"), bGrabbed ? TEXT("true") : TEXT("false"));
		if (!bGrabbed)
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		else
			InputComponent->BindAction("Release", IE_Pressed, this, &UGrabber::Release);
		//CheckInput();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent found in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	bGrabbed = true; // todo change bGrabbed only if an item is grabbed
	UE_LOG(LogTemp, Warning, TEXT("Grab btn pressed"));
	//UE_LOG(LogTemp, Warning, TEXT("bGrabbed = %s"), bGrabbed ? TEXT("true") : TEXT("false"));
}

void UGrabber::Release() {
	bGrabbed = false;
	UE_LOG(LogTemp, Warning, TEXT("Release btn pressed"));
	//UE_LOG(LogTemp, Warning, TEXT("bGrabbed = %s"), bGrabbed ? TEXT("true") : TEXT("false"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
	//	*PlayerViewPointLocation.ToString(),
	//	*PlayerViewPointRotation.ToString()
	//);

	FVector LineTracedEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTracedEnd,
		FColor(200,20,20),
		false,
		0.f,
		0.f,
		10.f
	);

	// Setup query parameters
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTracedEnd,
		ECollisionChannel::ECC_PhysicsBody,
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Line trace hits %s"), *ActorHit->GetName());
}

