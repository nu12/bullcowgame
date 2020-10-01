// Copyright nu12 2020.


#include "GrabbableActor.h"

// Sets default values
AGrabbableActor::AGrabbableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGrabbableActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UPrimitiveComponent* AGrabbableActor::GetGrabbableComponent() const
{
	return GrabbableComponent;
}
