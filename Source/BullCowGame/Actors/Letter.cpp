// Copyright nu12 2020.


#include "Letter.h"

ALetter::ALetter()
{
	AllStaticMeshes.AddDefaulted(26);


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	
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

void ALetter::SetLetterValue(TCHAR chr)
{
	LetterValue = chr;
}

TCHAR ALetter::GetLetterValue() const
{
	return LetterValue;
}

void ALetter::Launch()
{
	StaticMesh->AddForceAtLocation(StaticMesh->GetComponentRotation().Vector() * 5000.f, StaticMesh->GetComponentLocation());
}