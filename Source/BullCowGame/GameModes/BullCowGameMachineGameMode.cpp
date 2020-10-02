// Copyright nu12 2020.


#include "BullCowGameMachineGameMode.h"
#include "BullCowGame/Actors/BullCowGameMachine.h"
#include "BullCowGame/Actors/Letter.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Engine/TriggerVolume.h"
#include "BullCowGame/Characters/BullCowCharacter.h"


void ABullCowGameMachineGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ABullCowCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	// Load the words
	WordList.Emplace("AXE");
	WordList.Emplace("BOLD");
	WordList.Emplace("WITCH");
	WordList.Emplace("SWITCH");
	WordList.Emplace("RANDOMS");

	// Keep only isograms

	CharactersPool = FString("ABCDEFGHIJKLMNOPQRSTUVXZYW").GetCharArray();
	CharactersPool.RemoveAt(CharactersPool.Num() - 1); // Remove '\0'
}

void ABullCowGameMachineGameMode::HandleGameStart()
{
	// Set gameplay variables
	CurrentLevel = InitialLevel;
	TimeRemaining = InitialTime;
	bGameHasStarted = true;
	bSelectNewWord = true;

	// Wait letters spawning
	SetPlayerGrabAndInteract(false);
	
	HandleGameResume();
	GameStart();
}


void ABullCowGameMachineGameMode::HandleGameOver(bool PlayerWon)
{
	ClearTimer(TimeRemainingTimerHandle);
	SetPlayerGrabAndInteract(false);
	bGameIsOver = true;
	GameOver(PlayerWon);
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
	SetPlayerGrabAndInteract(true);
	bGameIsPaused = false;
	GameResume();
}


void ABullCowGameMachineGameMode::SelectNewWord()
{
	TArray<FString> WordsWithRightLength = GetWordsWithLength(CurrentLevel);
	
	HiddenWord = SelectRandomElementFromArray<FString>(WordsWithRightLength);

	// Spawn characters 
	CharactersToSpawn = HiddenWord.GetCharArray();
	CharactersToSpawn.RemoveAt(CharactersToSpawn.Num() - 1); // Remove '\0'

	// Spawn random characters
	int32 CharactersLeft = RandomCharactersPerTurn;
	while (CharactersLeft > 0)
	{
		TCHAR RandomChar = SelectRandomElementFromArray<TCHAR>(CharactersPool);
		if (CharactersToSpawn.Contains(RandomChar)) continue;
		CharactersToSpawn.Emplace(RandomChar);
		CharactersLeft--;
	}

	GetWorld()->GetTimerManager().SetTimer(SpawnLetterTimerHandle, this, &ABullCowGameMachineGameMode::SpawnNextLetter, LetterSpawnDelay, true);
}

void ABullCowGameMachineGameMode::HandleGamePause()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeRemainingTimerHandle);
	PlayerRef->SetCanGrab(false);
	bGameIsPaused = true;
	GamePause();

	bGuessIsCorrect = CheckGuess();

	if (bGuessIsCorrect && CurrentLevel == MaxLevel)
	{
		HandleGameOver(true);
		return;
	}

	if (bGuessIsCorrect)
	{
		CurrentLevel++;
		bSelectNewWord = true;
		TimeRemaining += TimeToAddWhenGuessIsCorrect;
		return;
	}
	CheckTimeIsOver(TimeRemaining -= TimeToRemoveWhenGuessIsWrong);
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
		TCHAR NextChar = SelectRandomElementFromArray(CharactersToSpawn);
		MachineRef->SpawnLetter(NextChar);
		SpawnedCharacters.Emplace(NextChar);
		CharactersToSpawn.Remove(NextChar);
		CharactersPool.Remove(NextChar);
		return;
	}
	ClearTimer(SpawnLetterTimerHandle);
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
	CheckTimeIsOver(--TimeRemaining);
}

void ABullCowGameMachineGameMode::CheckTimeIsOver(float Seconds)
{
	if (Seconds < 0)
	{
		HandleGameOver(false);
	}
}

bool ABullCowGameMachineGameMode::GetGameIsPaused() const
{
	return bGameIsPaused;
}

bool ABullCowGameMachineGameMode::GetGuessIsCorrect() const
{
	return bGuessIsCorrect;
}

void ABullCowGameMachineGameMode::SetPlayerGrabAndInteract(bool Value)
{
	PlayerRef->SetCanInteract(Value);
	PlayerRef->SetCanGrab(Value);
}

void ABullCowGameMachineGameMode::ClearTimer(FTimerHandle TimerToClear)
{
	GetWorld()->GetTimerManager().ClearTimer(TimeRemainingTimerHandle);
}

TArray<FString> ABullCowGameMachineGameMode::GetWordsWithLength(int32 Length)
{
	TArray<FString> WordsWithRightLength;
	for (FString Word : WordList)
	{
		if (Word.Len() == Length)
		{
			WordsWithRightLength.Emplace(Word);
		}
	}
	return WordsWithRightLength;
}