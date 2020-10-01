// Copyright nu12 2020.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableActor.h"
#include "Letter.generated.h"

class UProjectileMovementComponent;

UCLASS()
class BULLCOWGAME_API ALetter : public AGrabbableActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ALetter();

	void SetStaticMesh(int32 Index);

	TCHAR GetLetterValue() const;

	void SetLetterValue(TCHAR chr);

	void Launch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TCHAR LetterValue = 'A';
	UPROPERTY(EditAnywhere, Category = "Setup")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Setup")
		TArray<UStaticMesh*> AllStaticMeshes;

};
