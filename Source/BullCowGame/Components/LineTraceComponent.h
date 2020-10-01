// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULLCOWGAME_API ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULineTraceComponent();

	// https://forums.unrealengine.com/development-discussion/c-gameplay-programming/75367-is-it-possible-to-add-a-function-with-template-in-ue4
	template<class T>
	bool Perform(FHitResult& Hit, FVector& StartLocation, FVector& EndLocation, ECollisionChannel CollisionChannel = ECollisionChannel::ECC_WorldDynamic)
	{
		FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
		CollisionQueryParams.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, CollisionChannel, CollisionQueryParams);
		if (!Hit.GetActor()) return false;
		if (!Cast<T>(Hit.GetActor())) return false; // Can be nullptr
		return true;
	};

};
