// Copyright nu12 2020.


#include "Letter.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALetter::ALetter()
{
	AllStaticMeshes.AddDefaulted(26);


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileComponent->bAutoActivate = false;
	ProjectileComponent->InitialSpeed = 50000.f;
	ProjectileComponent->MaxSpeed = 50000.f;
	ProjectileComponent->bShouldBounce = true;
	ProjectileComponent->Bounciness = 0.2f;
}

void ALetter::BeginPlay()
{
	Super::BeginPlay();

}

void ALetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALetter::SetStaticMesh(int32 Index)
{
	if (!AllStaticMeshes[Index]) return;
	StaticMesh->SetStaticMesh(AllStaticMeshes[Index]);
}
void ALetter::Launch()
{
	StaticMesh->AddForceAtLocation(StaticMesh->GetComponentRotation().Vector() * ProjectileComponent->InitialSpeed, StaticMesh->GetComponentLocation());
}