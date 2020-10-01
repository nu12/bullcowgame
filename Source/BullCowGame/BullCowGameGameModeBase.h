// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BullCowGameGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class BULLCOWGAME_API ABullCowGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		virtual void HandleGameStart() {};

	UFUNCTION(BlueprintCallable)
		virtual void HandleGameOver(bool PlayerWon) {};

	UFUNCTION(BlueprintCallable)
		virtual void HandleGamePause() {};

	UFUNCTION(BlueprintCallable)
		virtual void HandleGameResume() {};

	UFUNCTION(BlueprintImplementableEvent)
		void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool PlayerWon);

	UFUNCTION(BlueprintImplementableEvent)
		void GamePause();

	UFUNCTION(BlueprintImplementableEvent)
		void GameResume();

};
