

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MovingPlatform.generated.h"

UCLASS()
class WEAPONMASTER_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Setup", meta = (MakeEditWidget = true))
		FVector EndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Setup")
		FVector StartPoint;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PlatformVisual;
	
	
};
