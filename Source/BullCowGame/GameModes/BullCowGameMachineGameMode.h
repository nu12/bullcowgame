// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "BullCowGame/BullCowGameGameModeBase.h"
#include "BullCowGameMachineGameMode.generated.h"

class ABullCowGameMachine;

UCLASS()
class BULLCOWGAME_API ABullCowGameMachineGameMode : public ABullCowGameGameModeBase
{
	GENERATED_BODY()

public:
	void HandleGameStart() override;

	void HandleGameOver(bool PlayerWon) override;

	void HandleGamePause() override;

	void HandleGameResume() override;

	void SelectNewWord();

	UFUNCTION(BlueprintCallable)
		void LeverActivation(ABullCowGameMachine* MachineRef);

	UFUNCTION(BlueprintCallable)
		int32 GetTimeRemaining() const;

protected:
	void BeginPlay() override;

private:
	bool bGameHasStarted = false;
	bool bGameIsOver = false;
	bool bGameIsPaused = true;
	bool bSelectNewWord = true;

	TArray<TCHAR> CharactersToSpawn;

	TArray<TCHAR> CharactersPool;

	TArray<TCHAR> SpawnedCharacters;

	TArray<FString> WordList;

	FString HiddenWord;

	int32 CurrentLevel = 0;

	int32 TimeRemaining = 0;

	void SpawnNextLetter();

	bool CheckGuess() const;

	ABullCowGameMachine* MachineRef = nullptr;

	FTimerHandle SpawnLetterTimerHandle;

	FTimerHandle TimeRemainingTimerHandle;

	void TimeTick();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 InitialTime = 120;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 TimeToAddWhenGuessIsCorrect = 60;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 TimeToRemoveWhenGuessIsWrong = 30;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 InitialLevel = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 MaxLevel = 7;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float LetterSpawnDelay = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 RandomCharactersPerTurn = 2;
};
