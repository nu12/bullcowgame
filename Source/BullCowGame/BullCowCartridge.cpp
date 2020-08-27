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
    if(bGameOver) {
        GenerateNewGame();
    }else if(Input.Equals("")){ 
        PrintLine(TEXT("Can't evaluate empty string."));
        AskForGuess();
    }else if(!IsIsogram(Input)){
        PrintLine(TEXT("The guess needs to be an isogram."));
        AskForGuess();
    }else if(Input.Equals(HiddenWord)){
        EndGameWithMessage("CONGRATULATIONS!! You've got it!");
    }else{
        if(PlayerHasLivesLeft()){
            PrintBullsAndCows(Input);
            AskForGuess();
        }else{
            EndGameWithMessage("You've lost!");
        }
    }
}

void UBullCowCartridge::AskForGuess() const{
    PrintLine(TEXT("Can you guess the %d letters word?"), HiddenWord.Len());
    PrintLine(TEXT("You have %d tries left."), Lives);
    PrintLine(TEXT("Enter your guess..."));
}

void UBullCowCartridge::GenerateNewGame(){
    bGameOver = false;
    HiddenWord = TEXT("above");
    Lives = HiddenWord.Len();
    PrintLine(TEXT("Welcome to Bull Cow Game!"));
    AskForGuess();
}

void UBullCowCartridge::EndGameWithMessage(const FString& Message){
    bGameOver = true;
    PrintLine(TEXT("%s"), *Message);
    PrintLine(TEXT("The word is: %s"), *HiddenWord);
    PrintLine(TEXT("Press Enter to play again."));
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

bool UBullCowCartridge::PlayerHasLivesLeft(){
    if (--Lives > 0) return true;
    return false;
}

bool UBullCowCartridge::IsIsogram(const FString& Input) const{
    for(int32 Index = 0; Index < Input.Len(); Index++)
        for(int32 Comparison = Index + 1; Comparison < Input.Len(); Comparison++)
            if(Input[Index] == Input[Comparison]) return false;
    return true;
}