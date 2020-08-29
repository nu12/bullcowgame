// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    /* IMPORTANT: In order for this to work in a packaged game 
    you would need to add the WordLists directory to the list 
    of Additional Non-Asset Directories to package in your 
    Project Settings under Packaging.*/
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    GenerateNewGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    if(bGameOver) {
        GenerateNewGame();
    }else{
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::GenerateNewGame(){
    bGameOver = false;
    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)];
    Lives = HiddenWord.Len();
    PrintLine(TEXT("Welcome to Bull Cow Game!"));
    AskForGuess();
}

void UBullCowCartridge::AskForGuess() const{
    PrintLine(TEXT("Can you guess the %d letters word?"), HiddenWord.Len());
    PrintLine(TEXT("You have %d tries left."), Lives);
    PrintLine(TEXT("Enter your guess..."));
}


void UBullCowCartridge::ProcessGuess(const FString& Guess){
    if(Guess.Equals("")){ 
        PrintLine(TEXT("Can't evaluate empty string."));
        AskForGuess();
    }else if(!IsIsogram(Guess)){
        PrintLine(TEXT("The guess needs to be an isogram."));
        AskForGuess();
    }else if(Guess.Equals(HiddenWord)){
        EndGameWithMessage("CONGRATULATIONS!! You've got it!");
    }else{
        if(PlayerHasLivesLeft()){
            PrintBullsAndCows(Guess);
            AskForGuess();
        }else{
            EndGameWithMessage("You've lost!");
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Input) const{
    for(int32 Index = 0; Index < Input.Len(); Index++)
        for(int32 Comparison = Index + 1; Comparison < Input.Len(); Comparison++)
            if(Input[Index] == Input[Comparison]) return false;
    return true;
}

bool UBullCowCartridge::PlayerHasLivesLeft(){
    if (--Lives > 0) return true;
    return false;
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

void UBullCowCartridge::EndGameWithMessage(const FString& Message){
    bGameOver = true;
    PrintLine(TEXT("%s"), *Message);
    PrintLine(TEXT("The word is: %s"), *HiddenWord);
    PrintLine(TEXT("Press Enter to play again."));
}