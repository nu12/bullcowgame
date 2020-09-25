// Copyright nu12 2020.


#include "BullCowGameMachine.h"

// Sets default values
ABullCowGameMachine::ABullCowGameMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("Spawn Point"));
	LetterSpawnPoint = CreateDefaultSubobject< USceneComponent>(TEXT("Letter Spawn Point"));
	RootComponent = Body;
	LetterSpawnPoint->SetupAttachment(Body);
}

// Called when the game starts or when spawned
void ABullCowGameMachine::BeginPlay()
{
	Super::BeginPlay();
	
}

