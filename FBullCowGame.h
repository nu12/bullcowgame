#pragma once

#include <string>

// To make syntax Unreal friendly
using FString = std::string;
using int32 = int;

struct FBullCowCount {
	int32 Bulls = 0;
	int32 Cows  = 0;
};

enum class EGuessStatus {
	OK,
	Not_isogram,
	Wrong_length,
	Not_Lowercase
};

class FBullCowGame {
public:
	FBullCowGame();

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	bool IsGameWon() const;
	
	void Reset();
	EGuessStatus CheckGuessValidity(FString);
	FBullCowCount SubmitValidGuess(FString);
private:
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bIsGameWon;

	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
};