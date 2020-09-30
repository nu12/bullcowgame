// Copyright nu12 2020.


#include "InteractionComponent.h"
#include "BullCowGame/Actors/InteractableActor.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInteractionComponent::Interaction()
{

	FHitResult Hit;
	FVector StartLocation;
	FRotator StartRotation;

	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(StartLocation, StartRotation);

	FVector EndLocation = StartLocation + StartRotation.Vector() * 200.f;


	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, Params);

	if (!Hit.GetActor()) return;


	if (!Cast<AInteractableActor>(Hit.GetActor())) return;

	Cast<AInteractableActor>(Hit.GetActor())->Interact();
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

