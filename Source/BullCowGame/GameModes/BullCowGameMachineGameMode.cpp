// Copyright nu12 2020.


#include "BullCowGameMachineGameMode.h"
#include "BullCowGame/Actors/BullCowGameMachine.h"
#include "BullCowGame/Actors/Letter.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Engine/TriggerVolume.h"


void ABullCowGameMachineGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Load the words

	CharacterPool = FString("ABCDEFGHIJKLMNOPQRSTUVXZYW").GetCharArray();
}

void ABullCowGameMachineGameMode::HandleGameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Game is has started"));

	CurrentLevel = InitialLevel;
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
		SelectNewWord();
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

void ABullCowGameMachineGameMode::SelectNewWord()
{
	HiddenWord = FString(TEXT("ABC"));
	CharactersToSpawn = HiddenWord.GetCharArray();
	CharactersToSpawn.RemoveAt(CharactersToSpawn.Num() - 1); // Remove '\0'

	// Spawn random characters
	int32 CharactersLeft = RandomCharactersPerTurn;
	while (CharactersLeft > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, CharacterPool.Num() - 1);
		TCHAR RandomChar = CharacterPool[RandomIndex];
		if (!CharactersToSpawn.Contains(RandomChar))
		{
			CharactersToSpawn.Emplace(RandomChar);
			CharactersLeft--;
			UE_LOG(LogTemp, Warning, TEXT("Random char: %c"), RandomChar);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(SpawnLetterTimerHandle, this, &ABullCowGameMachineGameMode::SpawnNextLetter, LetterSpawnDelay, true);
}

void ABullCowGameMachineGameMode::HandleGamePause()
{
	UE_LOG(LogTemp, Warning, TEXT("Game is paused"));
	bGameIsPaused = true;

	bool bIsGuessCorrect = CheckGuess();

	if (bIsGuessCorrect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guess is correct!!!"));
		CurrentLevel++;
		bSelectNewWord = true;
		// Add time
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Guess is INcorrect!!!"));
		// Remove Time
		// Turn lights
	}
}


void ABullCowGameMachineGameMode::LeverActivation(ABullCowGameMachine* MachineReference)
{
	if (!MachineRef) MachineRef = MachineReference;
	if (!bGameHasStarted) return HandleGameStart();
	return (bGameIsPaused) ? HandleGameResume() : HandleGamePause();
}

void ABullCowGameMachineGameMode::SpawnNextLetter()
{
	if (CharactersToSpawn.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, CharactersToSpawn.Num() - 1);
		TCHAR NextChar = CharactersToSpawn[RandomIndex];
		if (!SpawnedCharacters.Contains(NextChar))
		{
			SpawnedCharacters.Emplace(NextChar);
			MachineRef->SpawnLetter(NextChar);
		}

		CharactersToSpawn.Remove(NextChar);
		CharacterPool.Remove(NextChar);
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(SpawnLetterTimerHandle);
	bSelectNewWord = false;
	HandleGameResume();
}

bool ABullCowGameMachineGameMode::CheckGuess() const
{

	bool bIsGuessCorrect = true;

	for (int32 i = 0; i < HiddenWord.Len(); i++)
	{
		ATriggerVolume* Trigger = MachineRef->GetTriggerAt(i);
		TArray<AActor*> ActorsInTrigger;
		Trigger->GetOverlappingActors(ActorsInTrigger, ALetter::StaticClass());

		if (ActorsInTrigger.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: No actor in the trigger"), i);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Red);
			continue;
		}

		if (ActorsInTrigger.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: More than 1 actor in the trigger"), i);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Red);
			continue;
		}

		ALetter* Letter = Cast<ALetter>(ActorsInTrigger[0]);

		if (!Letter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Cast failed"), i);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Red);
			continue;
		}

		if (Letter->GetLetterValue() != HiddenWord[i])
		{
			FLinearColor LightColor = (HiddenWord.GetCharArray().Contains(Letter->GetLetterValue())) ? FLinearColor::Yellow : FLinearColor::Red;
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Letter incorrect %c x %c"), i, Letter->GetLetterValue(), HiddenWord[i]);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(LightColor);
			continue;
		}

		if (Letter->GetLetterValue() == HiddenWord[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Letter correct %c x %c"), i, Letter->GetLetterValue(), HiddenWord[i]);
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Green);
			continue;
		}

	}
	return bIsGuessCorrect;
}