// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabbableActor.generated.h"

UCLASS()
class BULLCOWGAME_API AGrabbableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabbableActor();

	UPrimitiveComponent* GetGrabbableComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPrimitiveComponent* GrabbableComponent = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
