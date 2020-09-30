// Copyright nu12 2020.


#include "GrabberComponent.h"
#include "Camera/CameraComponent.h"
#include "BullCowGame/Actors/GrabbableActor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrabberComponent::Grab()
{
	// Use Camera Locatioin & Rotation instead of GetActorEyesViewPoint
	UCameraComponent* Camera = Cast<UCameraComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));

	FHitResult Hit;
	FVector StartLocation = Camera->GetComponentLocation();
	FRotator StartRotation = Camera->GetComponentRotation();

	FVector EndLocation = StartLocation + StartRotation.Vector() * 200.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic, Params);

	if (!Hit.GetActor()) return;
	if (!Cast<AGrabbableActor>(Hit.GetActor())) return;

	//Cast<AGrabbableActor>(Hit.GetActor())->Interact();

	Cast<UPhysicsHandleComponent>(GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass()))->GrabComponentAtLocation(
		Hit.GetComponent(),
		NAME_None,
		Hit.GetActor()->GetActorLocation()
	);
	//TestFunc<UGrabberComponent>();
}

void UGrabberComponent::Release()
{
	Cast<UPhysicsHandleComponent>(GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass()))->ReleaseComponent();
}


//template< class T >
//T* UGrabberComponent::TestFunc()
//{
//	return T;
//}