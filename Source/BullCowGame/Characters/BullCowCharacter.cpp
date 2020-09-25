// Copyright nu12 2020.


#include "BullCowCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABullCowCharacter::ABullCowCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
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

}

// Called to bind functionality to input
void ABullCowCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABullCowCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABullCowCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABullCowCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ABullCowCharacter::LookRight);

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