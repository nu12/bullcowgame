// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Lever.generated.h"

class UCapsuleComponent;
class ABullCowGameMachine;
class ABullCowGameMachineGameMode;

UCLASS()
class BULLCOWGAME_API ALever : public AInteractableActor
{
	GENERATED_BODY()

public:
	ALever();
	void Interact() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = " ")
		UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, Category = " ")
		UStaticMeshComponent* Base;
	UPROPERTY(VisibleAnywhere, Category = " ")
		UStaticMeshComponent* Lever;

	UPROPERTY(EditAnywhere, Category = "Setup")
		FRotator StartRotation = FRotator(-20.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "Setup")
		FRotator EndRotation = FRotator(20.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Setup")
		ABullCowGameMachine* MachineRef = nullptr;

	ABullCowGameMachineGameMode* GameModeRef = nullptr;

	bool bReverse = false;
	bool bIsMoving = false;
};
