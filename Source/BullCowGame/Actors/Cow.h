// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cow.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UBillboardComponent;

UCLASS()
class BULLCOWGAME_API ACow : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACow();

	void Move();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, Category = " ")
		UBoxComponent* RootCapsule;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Setup")
		bool bCanMove = false;

	UPROPERTY(EditAnywhere, Category = "Setup")
		float ForceToApplyOnMove = 4000000.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
		float MinTimeToMove = 1.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
		float MaxTimeToMove = 4.f;

	FTimerHandle Timer;
};
