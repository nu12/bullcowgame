// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabberComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabberComponent();

	void Grab();
	void Release();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/*template<class T>
	T* TestFunc();*/

};
