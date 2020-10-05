// Copyright nu12 2020.


#include "BullCowGameMachineGameMode.h"
#include "BullCowGame/Actors/BullCowGameMachine.h"
#include "BullCowGame/Actors/Letter.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"
#include "Engine/TriggerVolume.h"
#include "BullCowGame/Characters/BullCowCharacter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


void ABullCowGameMachineGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ABullCowCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	LoadWordListFromFile();
	CharactersPool = GetCharArrayWithoutNullCharacter(FString("ABCDEFGHIJKLMNOPQRSTUVXZYW"));
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
	if (bSelectNewWord) return SelectNewWord();		

	// Set Lights
	for (int32 i = 0; i < HiddenWord.Len(); i++) TurnLightOnAt(i);
	
	// Resume game
	GetWorld()->GetTimerManager().SetTimer(TimeRemainingTimerHandle, this, &ABullCowGameMachineGameMode::TimeTick, 1.f, true);
	SetPlayerGrabAndInteract(true);
	bGameIsPaused = false;
	GameResume();
}


void ABullCowGameMachineGameMode::SelectNewWord()
{
	HiddenWord = SelectRandomElementFromArray<FString>(GetWordsWithLength(CurrentLevel));
	CharactersToSpawn = GetCharArrayWithoutNullCharacter(HiddenWord);
	AddRandomCharactersTo(CharactersToSpawn, RandomCharactersPerTurn);
	GetWorld()->GetTimerManager().SetTimer(SpawnLetterTimerHandle, this, &ABullCowGameMachineGameMode::SpawnNextLetter, LetterSpawnDelay, true);
}

void ABullCowGameMachineGameMode::HandleGamePause()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeRemainingTimerHandle);
	PlayerRef->SetCanGrab(false);
	bGameIsPaused = true;
	GamePause();

	bGuessIsCorrect = CheckGuess();

	if (bGuessIsCorrect && CurrentLevel == MaxLevel) return HandleGameOver(true);

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
		if(!SpawnedCharacters.Contains(NextChar))
		{
			MachineRef->SpawnLetter(NextChar);
			SpawnedCharacters.Emplace(NextChar);
		}
		CharactersToSpawn.Remove(NextChar);
		CharactersPool.Remove(NextChar);
		return;
	}
	ClearTimer(SpawnLetterTimerHandle);
	bSelectNewWord = false;
	HandleGameResume();
}

void ABullCowGameMachineGameMode::SetLightColorAt(int32 Index, FLinearColor Color)
{
	MachineRef->GetLightAt(Index)->GetLightComponent()->SetLightColor(Color);
}

bool ABullCowGameMachineGameMode::CheckGuess()
{
	bool bIsGuessCorrect = true;

	FLinearColor Color = FLinearColor::Red;

	for (int32 i = 0; i < HiddenWord.Len(); i++)
	{
		TArray<AActor*> ActorsInTrigger;
		MachineRef->GetTriggerAt(i)->GetOverlappingActors(ActorsInTrigger, ALetter::StaticClass());

		if (ActorsInTrigger.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: No actor in the trigger"), i);
			bIsGuessCorrect = false;
			SetLightColorAt(i, FLinearColor::Red);
			continue;
		}

		if (ActorsInTrigger.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: More than 1 actor in the trigger"), i);
			bIsGuessCorrect = false;
			SetLightColorAt(i, FLinearColor::Red);
			continue;
		}

		ALetter* Letter = Cast<ALetter>(ActorsInTrigger[0]);

		if (!Letter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Cast failed"), i);
			bIsGuessCorrect = false;
			SetLightColorAt(i, FLinearColor::Red);
			continue;
		}

		if (Letter->GetLetterValue() != HiddenWord[i])
		{
			FLinearColor LightColor = (HiddenWord.GetCharArray().Contains(Letter->GetLetterValue())) ? FLinearColor::Yellow : FLinearColor::Red;
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Letter incorrect %c x %c"), i, Letter->GetLetterValue(), HiddenWord[i]);
			bIsGuessCorrect = false;
			SetLightColorAt(i, LightColor);
			continue;
		}

		if (Letter->GetLetterValue() == HiddenWord[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Letter correct %c x %c"), i, Letter->GetLetterValue(), HiddenWord[i]);
			SetLightColorAt(i, FLinearColor::Green);
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
	GetWorld()->GetTimerManager().ClearTimer(TimerToClear);
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

void ABullCowGameMachineGameMode::LoadWordListFromFile()
{
	// Load the words
	TArray<FString> TempArray;
	const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
	FFileHelper::LoadFileToStringArray(TempArray, *WordListPath);

	// Keep only isograms
	for (FString Word : TempArray)
		if (IsIsogram(Word.ToUpper())) WordList.Emplace(Word.ToUpper());
}

bool ABullCowGameMachineGameMode::IsIsogram(const FString& Input) const {
	for (int32 Index = 0; Index < Input.Len(); Index++)
		for (int32 Comparison = Index + 1; Comparison < Input.Len(); Comparison++)
			if (Input[Index] == Input[Comparison]) return false;
	return true;
}

TArray<TCHAR> ABullCowGameMachineGameMode::GetCharArrayWithoutNullCharacter(FString StringRef)
{
	TArray<TCHAR>OutArray = StringRef.GetCharArray();
	OutArray.RemoveAt(OutArray.Num() - 1);
	return OutArray;
}

void ABullCowGameMachineGameMode::AddRandomCharactersTo(TArray<TCHAR>& InArray, int32 NumberOfCharsToAdd)
{
	int32 CharactersLeft = FMath::Min(NumberOfCharsToAdd, CharactersPool.Num());
	while (CharactersLeft > 0)
	{
		TCHAR RandomChar = SelectRandomElementFromArray<TCHAR>(CharactersPool);
		if (InArray.Contains(RandomChar)) continue;
		InArray.Emplace(RandomChar);
		CharactersLeft--;
	}

}

FString ABullCowGameMachineGameMode::GetHiddenWord() const
{
	return HiddenWord;
}

int32 ABullCowGameMachineGameMode::GetTimeToAddWhenGuessIsCorrect() const
{
	return TimeToAddWhenGuessIsCorrect;
}

int32 ABullCowGameMachineGameMode::GetTimeToRemoveWhenGuessIsWrong() const
{
	return TimeToRemoveWhenGuessIsWrong;
}

void ABullCowGameMachineGameMode::TurnLightOnAt(const int32& i)
{
	MachineRef->GetLightAt(i)->PointLightComponent->SetIntensity(LightIntensity);
}