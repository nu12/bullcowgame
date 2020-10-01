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


	bool bIsGuessCorrect = true;

	for (int32 i = 0; i < HiddenWord.Len(); i++)
	{
		ATriggerVolume* Trigger = MachineRef->GetTriggerAt(i);
		TArray<AActor*> ActorsInTrigger;
		Trigger->GetOverlappingActors(ActorsInTrigger, ALetter::StaticClass());
		
		UE_LOG(LogTemp, Warning, TEXT("Trigger: %s %d"), *Trigger->GetName(), ActorsInTrigger.Num());

		if (ActorsInTrigger.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: No actor in the trigger"), i);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Yellow);
			// Red light
			continue;
		}

		if (ActorsInTrigger.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: More than 1 actor in the trigger"), i);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Red);
			// Red light
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
			// If wrong position
				// Yellow light
				// continue
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Letter incorrect"), i);
			bIsGuessCorrect = false;
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Red);
			// Green light
			continue;
		}
		
		if (Letter->GetLetterValue() == HiddenWord[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Pos %d: Letter correct"), i);
			MachineRef->GetLightAt(i)->GetLightComponent()->SetLightColor(FLinearColor::Green);
			// Green light
			continue;
		}

		
		
		
	}

	if (bIsGuessCorrect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guess is correct!!!"));
		// Level ++
		// Select new word = true
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