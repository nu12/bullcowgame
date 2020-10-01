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

	TriggerVolumes.AddDefaulted(MaxLetters);
}

// Called when the game starts or when spawned
void ABullCowGameMachine::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < MaxLetters; i++)
	{
		SetupLight(i);
		SetupTriggerVolume(i);
	}
}

void ABullCowGameMachine::SetupLight(const int32& i)
{
	FString  SocketName = FString::Printf(TEXT("Light_%d"), i);
	APointLight* Light = GetWorld()->SpawnActor<APointLight>(APointLight::StaticClass(), Body->GetSocketTransform(FName(*SocketName)));
	Light->SetMobility(EComponentMobility::Movable);
	Light->PointLightComponent->SetAttenuationRadius(LightAttenuationRadius);
	Light->PointLightComponent->SetIntensity(0.f);
	Lights.Add(Light);
}

void ABullCowGameMachine::SetupTriggerVolume(const int32& i)
{
	if (!TriggerVolumes[i])
	{
		UE_LOG(LogTemp, Warning, TEXT("Pos %d: Trigger volume not found"), i);
		return;
	}
}

APointLight* ABullCowGameMachine::GetLightAt(int32 Index) const
{
	return Lights[Index];
}

ATriggerVolume* ABullCowGameMachine::GetTriggerAt(int32 Index) const
{
	return TriggerVolumes[Index];
}

void ABullCowGameMachine::SpawnLetter(TCHAR chr)
{
	if (!LetterClass) return;
	ALetter* SpawnedLetter = GetWorld()->SpawnActor<ALetter>(LetterClass, LetterSpawnPoint->GetComponentTransform());
	SpawnedLetter->SetStaticMesh((int32)(chr)-65);
	SpawnedLetter->SetLetterValue(chr);
	SpawnedLetter->Launch();
}
