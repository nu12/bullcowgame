// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "BullCowCharacter.generated.h"

class UPhysicsHandleComponent;
class ULineTraceComponent;

struct FCharacterReach
{
	FVector StartLocation;
	FVector EndLocation;

	FCharacterReach(UCameraComponent* CameraComponent, float Reach)
	{
		StartLocation = CameraComponent->GetComponentLocation();
		FRotator StartRotation = CameraComponent->GetComponentRotation();
		EndLocation = StartLocation + StartRotation.Vector() * Reach;
	}
};

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

	UFUNCTION(BlueprintCallable)
		void SetCanMove(bool Value);

	UFUNCTION(BlueprintCallable)
		void SetCanRotate(bool Value);

	UFUNCTION(BlueprintCallable)
		void SetCanInteract(bool Value);

	UFUNCTION(BlueprintCallable)
		void SetCanGrab(bool Value);

private:
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere)
		ULineTraceComponent* LineTraceComponent = nullptr;

	UPROPERTY(EditAnywhere)
		UPhysicsHandleComponent* PhysicsHandlerComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
		float Reach = 200.f; // 2m

	UPROPERTY(EditAnywhere, Category = "Setup")
		float RotationWhenGrabbed = 90.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		bool bCanMove = true;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		bool bCanRotate = true;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		bool bCanInteract = true;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		bool bCanGrab = true;
};
