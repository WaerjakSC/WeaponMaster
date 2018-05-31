

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MovingActor.generated.h"

UCLASS()
class WEAPONMASTER_API AMovingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USphereComponent* ProjectileCollision;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMoveVisual;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ShootyShoot;

	void DestroyItem();
	
	
};
