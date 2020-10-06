// Copyright nu12 2020.


#include "Cow.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACow::ACow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootCapsule = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = RootCapsule;
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACow::BeginPlay()
{
	Super::BeginPlay();

	if (bCanMove)
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACow::Move, MaxTimeToMove);
	}

}

void ACow::Move()
{
	RootCapsule->AddForceAtLocation((GetActorUpVector() + GetActorForwardVector()) * ForceToApplyOnMove, RootCapsule->GetComponentLocation());
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACow::Move, MaxTimeToMove);
}