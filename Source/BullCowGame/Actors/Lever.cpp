// Copyright nu12 2020.


#include "Lever.h"
#include "Components/CapsuleComponent.h"
#include "BullCowGame/GameModes/BullCowGameMachineGameMode.h"
#include "BullCowGameMachine.h"
#include "Misc/Char.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));

	RootComponent = Capsule;
	Base->SetupAttachment(Capsule);
	Lever->SetupAttachment(Capsule);
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	Lever->AddLocalRotation(StartRotation);
	GameModeRef = GetWorld()->GetAuthGameMode<ABullCowGameMachineGameMode>();
}

void ALever::Interact()
{
	if (!MachineRef) return;
	if (bIsMoving) return;
	GameModeRef->LeverActivation(MachineRef);
	bIsMoving = true;
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving) return;

	float PitchDirection = (bReverse) ? -1.f : 1.f;
	Lever->AddLocalRotation(FRotator(PitchDirection, 0.f, 0.f));

	if (Lever->GetComponentRotation().Pitch > EndRotation.Pitch) bReverse = true;
	if (Lever->GetComponentRotation().Pitch < StartRotation.Pitch)
	{
		bReverse = false;
		bIsMoving = false;
	}

}