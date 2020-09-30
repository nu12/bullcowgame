// Copyright nu12 2020.


#include "BullCowGameMachineGameMode.h"
#include "BullCowGame/Actors/BullCowGameMachine.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"


void ABullCowGameMachineGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Load the words
}

void ABullCowGameMachineGameMode::HandleGameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Game is has started"));

	// Set current level = initial level
	// Select new word = true

	bGameHasStarted = true;
	bSelectNewWord = true;
	HandleGameResume();
}


void ABullCowGameMachineGameMode::HandleGameOver()
{

}

void ABullCowGameMachineGameMode::HandleGameResume()
{
	if (bSelectNewWord)
	{
		HiddenWord = FString(TEXT("ABC"));
		HiddenWord.ToUpper();
		CharactersToSpawn = HiddenWord.GetCharArray();
		GetWorld()->GetTimerManager().SetTimer(SpawnLetterTimerHandle, this, &ABullCowGameMachineGameMode::SpawnNextLetter, 1.f, true);
		return;
	}

	// Set Lights
	for (int32 i = 0; i < HiddenWord.Len(); i++)
	{
		MachineRef->GetLightAt(i)->PointLightComponent->SetIntensity(100.f);
	}

	UE_LOG(LogTemp, Warning, TEXT("Game is resumed"));
	bGameIsPaused = false;
}

void ABullCowGameMachineGameMode::HandleGamePause()
{
	UE_LOG(LogTemp, Warning, TEXT("Game is paused"));
	bGameIsPaused = true;


	// If word is correct
		// Level ++
		// Select new word = true
		// Add time
	// Else
		// Remove Time
		// Turn lights
}


void ABullCowGameMachineGameMode::LeverActivation(ABullCowGameMachine* MachineReference)
{
	if (!MachineRef) MachineRef = MachineReference;
	if (!bGameHasStarted) return HandleGameStart();
	return (bGameIsPaused) ? HandleGameResume() : HandleGamePause();
}

void ABullCowGameMachineGameMode::SpawnNextLetter()
{
	if (CharactersToSpawn.Num() > 1)
	{
		TCHAR NextChar = CharactersToSpawn[0];
		CharactersToSpawn.RemoveAt(0);
		UE_LOG(LogTemp, Warning, TEXT("Spawn %d"), CharactersToSpawn.Num());
		UE_LOG(LogTemp, Warning, TEXT("Spawn %c"), NextChar);
		MachineRef->SpawnLetter(NextChar);
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(SpawnLetterTimerHandle);
	bSelectNewWord = false;
	HandleGameResume();
}