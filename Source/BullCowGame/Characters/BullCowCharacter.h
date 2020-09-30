// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BullCowCharacter.generated.h"

class UCameraComponent;
class UInteractionComponent;
class UGrabberComponent;
class UPhysicsHandleComponent;

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

	void Interact();

	void Grab();

	void Release();

private:
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere)
		UInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(EditAnywhere)
		UGrabberComponent* GrabberComponent = nullptr;

	UPROPERTY(EditAnywhere)
		UPhysicsHandleComponent* PhysicsHandlerComponent = nullptr;
};
