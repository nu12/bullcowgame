#pragma once

#include "FBullCowGame.h"
#include <map>

// To make syntax Unreal friendly
#define TMap std::map

const FString HIDDEN_WORD = "planes"; // this MUST be an isogram

FBullCowGame::FBullCowGame() {
	Reset();
}

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bIsGameWon; }

int32 FBullCowGame::GetMaxTries() const { 
	TMap<int32, int32> WordLengthToTries{
		{3,4},
		{4,7},
		{5,10},
		{6,15},
		{7,20}
	};
	return WordLengthToTries[GetHiddenWordLength()];
}

void FBullCowGame::Reset() {
	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bIsGameWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) {
	if ( ! IsIsogram(Guess))
		return EGuessStatus::Not_isogram;
	else if (! IsLowercase(Guess))
		return EGuessStatus::Not_Lowercase;
	else if (Guess.length() != GetHiddenWordLength())
		return EGuessStatus::Wrong_length;
	return EGuessStatus::OK;
}

FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess) {
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	for (size_t i = 0; i < MyHiddenWord.length(); i++) 
		for (size_t j = 0; j < MyHiddenWord.length(); j++)
			if (Guess[i] == MyHiddenWord[j]) 
				if (i == j) 
					BullCowCount.Bulls++;				
				else 
					BullCowCount.Cows++;

	if (BullCowCount.Bulls == GetHiddenWordLength()) {
		bIsGameWon = true;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const {
	if (Word.length() <= 1) { return true; }
	TMap<char, bool> LetterSeen;
	for (auto Letter : Word) {
		Letter = tolower(Letter);
		if (LetterSeen[Letter])
			return false;
		else
			LetterSeen[Letter] = true;
	}
	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const {
	for (auto Letter : Word) {
		if (Letter != tolower(Letter)) {
			return false;
		}
	}
	return true;
}
