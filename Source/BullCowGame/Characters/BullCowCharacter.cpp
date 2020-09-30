// Copyright nu12 2020.


#include "BullCowCharacter.h"
#include "Camera/CameraComponent.h"
#include "BullCowGame/Components/InteractionComponent.h"
#include "BullCowGame/Components/GrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
ABullCowCharacter::ABullCowCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	GrabberComponent = CreateDefaultSubobject<UGrabberComponent>(TEXT("Grabber Component"));
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
		FHitResult Hit;
		FVector StartLocation;
		FRotator StartRotation;

		GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(StartLocation, StartRotation);

		FVector EndLocation = StartLocation + StartRotation.Vector() * 200.f;
		UPrimitiveComponent* Component = PhysicsHandlerComponent->GetGrabbedComponent();
		//Component->SetWorldRotation(FRotator(70.f, GetActorRotation().Yaw, GetActorRotation().Roll));
		PhysicsHandlerComponent->SetTargetLocation(EndLocation);
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
	InteractionComponent->Interaction();
}

void ABullCowCharacter::Grab()
{
	GrabberComponent->Grab();
}

void ABullCowCharacter::Release()
{
	GrabberComponent->Release();
}
