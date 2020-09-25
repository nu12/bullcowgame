// Copyright nu12 2020.


#include "InteractableActor.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();	
}


void AInteractableActor::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact() needs implementation in child class."));
}