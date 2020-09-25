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
	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* Billboard;
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ActionWidget;

	UPROPERTY(EditAnywhere)
	bool bCanMove = false;

	UPROPERTY(EditAnywhere)
	FString TextToDisplay = TEXT("");

	FTimerHandle Timer;
};
