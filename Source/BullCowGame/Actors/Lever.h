// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Lever.generated.h"

class UCapsuleComponent;

UCLASS()
class BULLCOWGAME_API ALever : public AInteractableActor
{
	GENERATED_BODY()

public:
	ALever();
	void Interact() override;

private:
	UPROPERTY(VisibleAnywhere, Category = " ")
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, Category = " ")
	UStaticMeshComponent* Base;
	UPROPERTY(VisibleAnywhere, Category = " ")
	UStaticMeshComponent* Lever;
};
