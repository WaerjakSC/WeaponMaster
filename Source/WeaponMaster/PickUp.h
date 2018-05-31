

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "PickUp.generated.h"

class ACometGameMode;
class AProtoChar;
UCLASS()
class WEAPONMASTER_API APickUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


	// Called to start a timeline to rotate the item
	UFUNCTION(BlueprintImplementableEvent)
		void RotateActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potato Variables")
		UBoxComponent* PotatoCollider;
	UPROPERTY(EditAnywhere, Category = "Potato Variables")
		UStaticMeshComponent* PotatoVisual;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potato Variables")
		UMaterial* PotatoMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "External Objects")
		class AActor* Crystal;
	UPROPERTY(BlueprintReadWrite)
		FVector StartPoint;
	UPROPERTY(BlueprintReadWrite)
		FVector EndPoint;
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
		void StartMovement();
};
