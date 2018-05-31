

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "WeightTrigger.generated.h"
/**
* Step on actor to activate/deactivate something
* Flat box collision that checks for weight
* Use timeline or similar C++ version to get smooth movement down/up
*/
class AProtoChar;
UCLASS()
class WEAPONMASTER_API AWeightTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeightTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Trigger Components")
		USphereComponent* SphereTrigger;

	UFUNCTION(BlueprintImplementableEvent)
		void OverlappedPlayer();
	UPROPERTY(BlueprintReadOnly)
		FVector StartPos;
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void HidePlatform();
};
