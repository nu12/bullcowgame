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

    if(HiddenWord.Len() != Guess.Len()){
        PrintLine(TEXT("Enter a word of %d letters."), HiddenWord.Len());
        AskForGuess();
        return; // Do not lose lives
    }

    if(!IsIsogram(Guess)){
        PrintLine(TEXT("The guess needs to be an isogram."));
        AskForGuess();
        return; // Do not lose lives
    }

    if(Guess.Equals(HiddenWord)){
        EndGameWithMessage("CONGRATULATIONS!! You've got it!");
        return; // Do not AskForGuess here
    }
    
    if(!PlayerHasLivesLeft()){
        EndGameWithMessage("You've lost!");
        return; // Do not AskForGuess here
    }
             
    PrintBullsAndCows(Guess);
    AskForGuess();
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

void UBullCowCartridge::PrintBullsAndCows(const FString& Guess) const{
    int32 Bulls, Cows;
    GetBullsAndCowsCount(Guess, Bulls, Cows); // Using out parameters
    PrintLine(TEXT("Bulls: %d | Cows: %d."), Bulls, Cows);
    PrintLine(TEXT("Bull: right letter, right position."));
    PrintLine(TEXT("Cow: right letter, wrong position."));
}


void UBullCowCartridge::GetBullsAndCowsCount(const FString& Guess, int32& BullCount, int32& CowCount) const{ // Out parameters (return)
    BullCount = 0;
    CowCount = 0;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex]){
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < Guess.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex]){
                CowCount++;
                break;
            }
        }
    }

}

void UBullCowCartridge::EndGameWithMessage(const FString& Message){
    bGameOver = true;
    PrintLine(TEXT("%s"), *Message);
    PrintLine(TEXT("The word is: %s"), *HiddenWord);
    PrintLine(TEXT("Press Enter to play again."));
}