// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BullCowGameMachine.generated.h"

UCLASS()
class BULLCOWGAME_API ABullCowGameMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullCowGameMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = " ")
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, Category = " ")
	USceneComponent* LetterSpawnPoint;
	//Add sockets
	//TArray triggervolume

};
