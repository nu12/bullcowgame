#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// To make syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame;

int main(void) {
	do {
		PrintIntro();
		PlayGame();
	} while (AskToPlayAgain());
	return 0;
}

void PrintIntro() {	
	std::cout << "Welcome to Bulls and Cows, a fun word game." << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I`m thinking of?" << std::endl;
	return;
}

void PlayGame() {
	BCGame.Reset();
	for (int32 i = 0; i < BCGame.GetMaxTries(); i++) {
		FText Guess = GetValidGuess();
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		std::cout << "Bulls = " << BullCowCount.Bulls << " | Cows = " << BullCowCount.Cows << std::endl << std::endl;

		if (BCGame.IsGameWon())
			break;
	}
	PrintGameSummary();
	return;
}

FText GetValidGuess() {
	FText Guess;
	EGuessStatus Status;
	do {
		std::cout << "Try " << BCGame.GetCurrentTry() << " of " << BCGame.GetMaxTries() << ". Enter your guess: ";
		std::getline(std::cin, Guess);
		Status = BCGame.CheckGuessValidity(Guess);

		switch (Status) {
		case EGuessStatus::Not_isogram:
			std::cout << "Please enter an isogram." << std::endl << std::endl;
			break;
		case EGuessStatus::Wrong_length:
			std::cout << "Please enter a "<< BCGame.GetHiddenWordLength() <<" letters word." << std::endl << std::endl;
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter lowercase word." << std::endl << std::endl;
			break;
		}
	} while (Status != EGuessStatus::OK);
	
	return Guess;
}

bool AskToPlayAgain() {
	std::cout << "Do you want to play again (y/N)? ";
	FText resp = "";
	std::getline(std::cin, resp);
	std::cout << std::endl << std::endl;
	return resp[0] == 'y' || resp[0] == 'Y';
}

void PrintGameSummary() {
	if (BCGame.IsGameWon()) {
		std::cout << "You win! Congratulations!" << std::endl;
	}
	else {
		std::cout << "Better luck next time!" << std::endl;
	}
	return;
}
