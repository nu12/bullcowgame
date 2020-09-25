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
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	ActionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	RootComponent = RootCapsule;
	StaticMesh->SetupAttachment(RootComponent);
	Billboard->SetupAttachment(RootComponent);
	ActionWidget->SetupAttachment(Billboard);
}

// Called when the game starts or when spawned
void ACow::BeginPlay()
{
	Super::BeginPlay();
	
	if (bCanMove)
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACow::Move, 3.f);
	}
	
}

void ACow::Move()
{
	RootCapsule->AddForceAtLocation((GetActorUpVector() + GetActorForwardVector()) * 4000000, RootCapsule->GetComponentLocation());
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACow::Move, 3.f);
}