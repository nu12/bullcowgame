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
	WordList.Emplace("AXE");
	WordList.Emplace("BOLD");
	WordList.Emplace("WITCH");
	WordList.Emplace("SWITCH");
	WordList.Emplace("RANDOMS");

	CharactersPool = FString("ABCDEFGHIJKLMNOPQRSTUVXZYW").GetCharArray();
	CharactersPool.RemoveAt(CharactersPool.Num() - 1); // Remove '\0'
}

void ABullCowGameMachineGameMode::HandleGameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Game is has started"));

	CurrentLevel = InitialLevel;
	TimeRemaining = InitialTime;

	bGameHasStarted = true;
	bSelectNewWord = true;
	HandleGameResume();
}


void ABullCowGameMachineGameMode::HandleGameOver(bool PlayerWon)
{
	bGameIsOver = true;
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
	GetWorld()->GetTimerManager().SetTimer(TimeRemainingTimerHandle, this, &ABullCowGameMachineGameMode::TimeTick, 1.f, true);
	bGameIsPaused = false;
	GameResume();
}

void ABullCowGameMachineGameMode::SelectNewWord()
{
	// Create array of words where Len == CurrentLevel
	TArray<FString> WordsWithRightLength;
	for (FString Word : WordList)
	{
		if (Word.Len() == CurrentLevel)
		{
			WordsWithRightLength.Emplace(Word);
		}
	}

	// Select word
	int32 WordRandomIndex = FMath::RandRange(0, WordsWithRightLength.Num() - 1);
	HiddenWord = WordsWithRightLength[WordRandomIndex];

	// Spawn characters 
	CharactersToSpawn = HiddenWord.GetCharArray();
	CharactersToSpawn.RemoveAt(CharactersToSpawn.Num() - 1); // Remove '\0'

	// Spawn random characters
	int32 CharactersLeft = RandomCharactersPerTurn;
	while (CharactersLeft > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, CharactersPool.Num() - 1);
		TCHAR RandomChar = CharactersPool[RandomIndex];
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
	GetWorld()->GetTimerManager().ClearTimer(TimeRemainingTimerHandle);
	bGameIsPaused = true;

	bool bIsGuessCorrect = CheckGuess();

	if (bIsGuessCorrect && CurrentLevel == MaxLevel)
	{
		HandleGameOver(true);
		return;
	}

	if (bIsGuessCorrect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guess is correct!!!"));
		CurrentLevel++;
		bSelectNewWord = true;
		TimeRemaining += TimeToAddWhenGuessIsCorrect;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Guess is INcorrect!!!"));
		TimeRemaining -= TimeToRemoveWhenGuessIsWrong;
	}
}


void ABullCowGameMachineGameMode::LeverActivation(ABullCowGameMachine* MachineReference)
{
	if (!MachineRef) MachineRef = MachineReference;
	if (!bGameHasStarted) return HandleGameStart();
	if (bGameIsOver) return;
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
		CharactersPool.Remove(NextChar);
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

int32 ABullCowGameMachineGameMode::GetTimeRemaining() const
{
	return TimeRemaining;
}

void ABullCowGameMachineGameMode::TimeTick()
{
	TimeRemaining--;
	if (TimeRemaining < 0)
	{
		HandleGameOver(false);
	}
}