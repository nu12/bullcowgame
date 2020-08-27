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

	void AskForGuess() const;
	void GenerateNewGame();
	void EndGameWithMessage(const FString& Message);
	void PrintBullsAndCows(const FString& Input);
	bool PlayerHasLivesLeft();
	bool IsIsogram(const FString& Input) const;
};
