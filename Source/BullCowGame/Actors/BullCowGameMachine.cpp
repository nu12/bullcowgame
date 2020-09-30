// Copyright nu12 2020.


#include "BullCowGameMachine.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Engine/TriggerVolume.h"
#include "Letter.h"

#include "DrawDebugHelpers.h"

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

	for (int32 i = 0; i < MaxLetters; i++)
	{
		// Light Setup
		FString  SocketName = FString::Printf(TEXT("Light_%d"), i);
		APointLight* Light = GetWorld()->SpawnActor<APointLight>(APointLight::StaticClass(), Body->GetSocketTransform(FName(*SocketName)));
		Light->SetMobility(EComponentMobility::Movable);
		Light->PointLightComponent->SetAttenuationRadius(20.f);
		Light->PointLightComponent->SetIntensity(0.f);
		Lights.Add(Light);

		// TriggerVolume Setup
		SocketName = FString::Printf(TEXT("Trigger_%d"), i);
		ATriggerVolume* Trigger = GetWorld()->SpawnActor<ATriggerVolume>(ATriggerVolume::StaticClass(), Body->GetSocketTransform(FName(*SocketName)));
		Trigger->SetActorScale3D(FVector(0.175f, 0.175f, 0.175f));
		TriggerVolumes.Add(Trigger);

		DrawDebugBox(GetWorld(), Trigger->GetActorLocation(), Trigger->GetActorScale() * 100, FColor::Cyan, true, -1, 0, 5);
	}
}

APointLight* ABullCowGameMachine::GetLightAt(int32 Index) const
{
	return Lights[Index];
}

void ABullCowGameMachine::SpawnLetter(TCHAR chr)
{
	if (!LetterClass) return;
	ALetter* SpawnedLetter = GetWorld()->SpawnActor<ALetter>(LetterClass, LetterSpawnPoint->GetComponentTransform());
	SpawnedLetter->SetStaticMesh((int32)(chr)-65);
	//SpawnedLetter->SetLetterValue(chr);
	SpawnedLetter->Launch();
}
