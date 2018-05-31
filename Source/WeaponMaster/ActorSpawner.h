

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "ActorSpawner.generated.h"

class AWeightTrigger;
class AMovingActor;

UCLASS()
class WEAPONMASTER_API AActorSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StaggeredSpawningOne();
	UFUNCTION(BlueprintCallable)
	void StaggeredSpawningTwo();
	UFUNCTION(BlueprintCallable)
	void ConstantSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Setup")
	UStaticMeshComponent* DummyRoot;

	// Don't set Y or Z to anything but 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Setup", meta = (MakeEditWidget = true))
		FVector EndPoint;
	UPROPERTY(EditAnywhere, Category = "Type of switch activator")
		TSubclassOf<AWeightTrigger> SwitchActivator;
	UPROPERTY(EditAnywhere, Category = "Spawned Actor")
		TSubclassOf<AMovingActor> SpawnedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Options")
		bool StaggeredSpawns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Options")
		float SpawnDelay = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Options")
		float StaggeredDelay = 6;
	UPROPERTY(BlueprintReadOnly)
		float MeshWidth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool SpawnNow;
	UFUNCTION(BlueprintImplementableEvent)
		void Trapped();
	UFUNCTION(BlueprintImplementableEvent)
		void BPStaggered();
	FVector RootLocation;
	


	// Additional distance between spawned meshes
	UPROPERTY(EditAnywhere, Category = "Actor Setup")
		float ExtraDistance;

	FVector RelativeExtra = FVector(100.f, 0.f, 0.f);

	const float SpawnDelayFirst = SpawnDelay;
	TArray<UStaticMeshComponent*>MeshArray;

	FVector IgnoreZ = FVector(1.f, 1.f, 0.f);

	FVector RelativeLoc = FVector(0.f, 0.f, 0.f);

	float VectorSize;

	float TempMesh;
	FVector MeshExtentMin;
	FVector MeshExtentMax;
	FVector MeshExtent;
	FVector MeshScale;
	FTransform MeshTransform;
	int32 numMeshSpawn;


};
