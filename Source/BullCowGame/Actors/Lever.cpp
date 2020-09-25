// Copyright nu12 2020.


#include "Lever.h"
#include "Components/CapsuleComponent.h"

ALever::ALever()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));

	RootComponent = Capsule;
	Base->SetupAttachment(Capsule);
	Lever->SetupAttachment(Capsule);
}

void ALever::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Lever, needs implementation."));
}