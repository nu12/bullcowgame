// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
	int32 Lives;
	bool bGameOver;
	TArray<FString> Words;
	FString HiddenWord;

	void GenerateNewGame();
	void AskForGuess() const;
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Input) const;
	bool PlayerHasLivesLeft();
	void PrintBullsAndCows(const FString& Guess) const;
	void GetBullsAndCowsCount(const FString& Guess, int32& BullCount, int32& CowCount) const;
	void EndGameWithMessage(const FString& Message);
};
