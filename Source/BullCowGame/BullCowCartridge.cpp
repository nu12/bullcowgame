// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
//#include "Engine/Engine.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    GenerateNewGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    if(Lives == 0 && !Input.Equals("yes")){ 
        FGenericPlatformMisc::RequestExit(false);
    }else if(Lives == 0 && Input.Equals("yes")) {
        GenerateNewGame();
    }else if(Input.Equals("")){ 
        PrintLine(TEXT("Can't evaluate empty string."));
        AskForGuess();
    }
    else if(Input.Equals(HiddenWord)){
        PrintLine(TEXT("CONGRATULATIONS!! You got it!"));
        Lives = 0;
        PromptToPlayAgain();
    }else{
        Lives--;
        if(Lives == 0){
            PrintLine(TEXT("You have lost!"));
            PrintLine(TEXT("The word is: %s"), *HiddenWord);
            PromptToPlayAgain();
        }else{
            PrintBullsAndCows(Input);
            AskForGuess();
        }
    }
}

void UBullCowCartridge::AskForGuess(){
    PrintLine(TEXT("Can you guess the %d letter word?"), HiddenWord.Len());
    PrintLine(TEXT("You have %d tries left."), Lives);
    PrintLine(TEXT("Enter your guess..."));
}

void UBullCowCartridge::GenerateNewGame(){
    PrintLine(TEXT("Welcome to Bull Cow Game!"));
    HiddenWord = TEXT("above");
    Lives = HiddenWord.Len();
    AskForGuess();
}

void UBullCowCartridge::PromptToPlayAgain(){
    PrintLine(TEXT("Write \"yes\" to play again."));
}

void UBullCowCartridge::PrintBullsAndCows(const FString& Input){
    int32 Cows = 0;
    int32 Bulls = 0;
    TArray<TCHAR> InputArray = Input.GetCharArray();
    TArray<TCHAR> HiddenWordArray = HiddenWord.GetCharArray();
    for(int32 i = 0; i < Input.Len(); i++){
        for(int32 j = 0; j < HiddenWord.Len(); j++){
            if(InputArray[i] == HiddenWordArray[j]){
                if(i == j){
                    Bulls++;
                }else{
                    Cows++;
                }
                HiddenWordArray[j] = ' ';
            }
        }
    }
    PrintLine(TEXT("Bulls: %d | Cows: %d."), Bulls, Cows);
    PrintLine(TEXT("A Bull is the right letter in the right position."));
    PrintLine(TEXT("A Cow is the right letter in the wrong position."));
}