// Copyright nu12 2020.


#include "BullCowCharacter.h"
#include "BullCowGame/Components/LineTraceComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "BullCowGame/Actors/GrabbableActor.h"
#include "BullCowGame/Actors/InteractableActor.h"

// Sets default values
ABullCowCharacter::ABullCowCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	LineTraceComponent = CreateDefaultSubobject<ULineTraceComponent>(TEXT("Line Trace Component"));
	PhysicsHandlerComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handler Component"));
}

// Called when the game starts or when spawned
void ABullCowCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullCowCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PhysicsHandlerComponent->GetGrabbedComponent())
	{
		FCharacterReach CharacterReach(CameraComponent, Reach);
		PhysicsHandlerComponent->SetTargetLocationAndRotation(CharacterReach.EndLocation, FRotator(GetActorRotation().Pitch + RotationWhenGrabbed, GetActorRotation().Yaw, GetActorRotation().Roll));
	}

}

// Called to bind functionality to input
void ABullCowCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABullCowCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABullCowCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABullCowCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ABullCowCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ABullCowCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Grab"), IE_Pressed, this, &ABullCowCharacter::Grab);
	PlayerInputComponent->BindAction(TEXT("Release"), IE_Released, this, &ABullCowCharacter::Release);
}

void ABullCowCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ABullCowCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ABullCowCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABullCowCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value);
}

void ABullCowCharacter::Interact()
{
	FCharacterReach CharacterReach(CameraComponent, Reach);
	AInteractableActor* ActorToInteract = LineTraceComponent->Perform<AInteractableActor>(CharacterReach.StartLocation, CharacterReach.EndLocation);
	if (ActorToInteract)
	{
		ActorToInteract->Interact();
	}
}

void ABullCowCharacter::Grab()
{
	FCharacterReach CharacterReach(CameraComponent, Reach);
	AGrabbableActor* ActorToGrab = LineTraceComponent->Perform<AGrabbableActor>(CharacterReach.StartLocation, CharacterReach.EndLocation);
	if (ActorToGrab)
	{
		PhysicsHandlerComponent->GrabComponentAtLocationWithRotation(
			ActorToGrab->GetGrabbableComponent(),
			NAME_None,
			ActorToGrab->GetActorLocation(),
			ActorToGrab->GetActorRotation()
		);
	}
}

void ABullCowCharacter::Release()
{
	PhysicsHandlerComponent->ReleaseComponent();
}
