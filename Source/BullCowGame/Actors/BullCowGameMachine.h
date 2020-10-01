// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BullCowGameMachine.generated.h"

class ATriggerVolume;
class APointLight;
class ALetter;

UCLASS()
class BULLCOWGAME_API ABullCowGameMachine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullCowGameMachine();

	void SpawnLetter(TCHAR chr);

	UFUNCTION(BlueprintCallable)
	APointLight* GetLightAt(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	ATriggerVolume* GetTriggerAt(int32 Index) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = " ")
		UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, Category = " ")
		USceneComponent* LetterSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Setup")
		TArray<ATriggerVolume*> TriggerVolumes;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
		TArray<APointLight*> Lights;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		int32 MaxLetters = 8;

	UPROPERTY(EditAnywhere, Category = "Setup")
		TSubclassOf<ALetter> LetterClass;
};
