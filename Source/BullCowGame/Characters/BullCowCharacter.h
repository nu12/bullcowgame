// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BullCowCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BULLCOWGAME_API ABullCowCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABullCowCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void LookUp(float Value);

	void LookRight(float Value);

private:
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent = nullptr;
};
