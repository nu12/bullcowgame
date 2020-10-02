// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "BullCowGame/BullCowGameGameModeBase.h"
#include "BullCowGameMachineGameMode.generated.h"

class ABullCowGameMachine;
class ABullCowCharacter;

UCLASS()
class BULLCOWGAME_API ABullCowGameMachineGameMode : public ABullCowGameGameModeBase
{
	GENERATED_BODY()

public:
	void HandleGameStart() override;

	void HandleGameOver(bool PlayerWon) override;

	void HandleGamePause() override;

	void HandleGameResume() override;

	TArray<FString> GetWordsWithLength(int32 Length);

	void SelectNewWord();

	UFUNCTION(BlueprintCallable)
		void LeverActivation(ABullCowGameMachine* MachineRef);

	UFUNCTION(BlueprintCallable)
		int32 GetTimeRemaining() const;

	UFUNCTION(BlueprintCallable)
		bool GetGameIsPaused() const;

	UFUNCTION(BlueprintCallable)
		bool GetGuessIsCorrect() const;

	void SetPlayerGrabAndInteract(bool Value);

	void ClearTimer(FTimerHandle TimerToClear);

protected:
	void BeginPlay() override;

private:
	bool bGameHasStarted = false;
	bool bGameIsOver = false;
	bool bGameIsPaused = true;
	bool bSelectNewWord = true;
	bool bGuessIsCorrect = true;

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
	ABullCowCharacter* PlayerRef = nullptr;

	FTimerHandle SpawnLetterTimerHandle;

	FTimerHandle TimeRemainingTimerHandle;

	void TimeTick();

	void CheckTimeIsOver(float Seconds);

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

	template <class T>
	T SelectRandomElementFromArray(TArray<T> InArray)
	{
		int32 RandomIndex = FMath::RandRange(0, InArray.Num() - 1);
		return InArray[RandomIndex];
	};
};
