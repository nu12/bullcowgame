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

	void HandleGameOver() override;

	void HandleGamePause() override;

	void HandleGameResume() override;

	void SelectNewWord();

	UFUNCTION(BlueprintCallable)
		void LeverActivation(ABullCowGameMachine* MachineRef);

protected:
	void BeginPlay() override;

private:
	bool bGameHasStarted = false;
	bool bGameIsPaused = true;
	bool bSelectNewWord = true;

	TArray<TCHAR> CharactersToSpawn;

	TArray<TCHAR> CharacterPool;

	TArray<TCHAR> SpawnedCharacters;

	void SpawnNextLetter();

	bool CheckGuess() const;

	ABullCowGameMachine* MachineRef = nullptr;

	FString HiddenWord;

	FTimerHandle SpawnLetterTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 InitialLevel = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 MaxLevel = 7;

	int32 CurrentLevel = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float LetterSpawnDelay = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int32 RandomCharactersPerTurn = 2;
};
