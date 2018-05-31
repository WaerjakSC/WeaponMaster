// TODO: Add option to let someone choose number or meshes instead of or in addition to endpoint


#include "ActorSpawner.h"
#include "WeightTrigger.h"
#include "MovingActor.h"


// Sets default values
AActorSpawner::AActorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DummyRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Start Point"));
	RootComponent = DummyRoot;

}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	// Get size of the static mesh
	DummyRoot->UpdateBounds();
	DummyRoot->GetLocalBounds(MeshExtentMin, MeshExtentMax);
	MeshScale = GetActorScale();
	RootLocation = GetActorLocation();
	MeshExtent = ((MeshExtentMax - MeshExtentMin));
	MeshWidth = MeshExtent.X;
	// Multiply by 0 in Z axis to ignore vertical values
	EndPoint = EndPoint * IgnoreZ;
	// Get the length of the vector
	VectorSize = EndPoint.Size();
	// Divide the length of the vector by the width of the mesh
	TempMesh = VectorSize / MeshWidth;
	// Round up to change into integer
	numMeshSpawn = FMath::RoundHalfFromZero(TempMesh);
	numMeshSpawn--;
	RelativeExtra.X += ExtraDistance;
	UE_LOG(LogTemp, Warning, TEXT("x: %d"), numMeshSpawn)
		if (numMeshSpawn > 0)
		{
			for (int32 i{ 0 }; i < numMeshSpawn; i++)
			{
				// Currently some kind of problem related to scale and world axises not being the same (When rotated)
				FString j = "Mesh" + i;
				MeshArray.Add(NewObject<UStaticMeshComponent>(this, *FString("Mesh" + FString::FromInt(i))));
				MeshArray[i]->SetupAttachment(DummyRoot);
				RelativeLoc.X = MeshWidth * (i + 1);
				MeshArray[i]->SetRelativeLocation(RelativeLoc + (RelativeExtra  * (i + 1)));
				MeshArray[i]->SetStaticMesh(DummyRoot->GetStaticMesh());
				MeshArray[i]->RegisterComponentWithWorld(GetWorld());
			}
		}
		else
		{
			// Not sure what to do here yet
		}
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SwitchActivator)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Time remaining: %f"), GetWorldTimerManager().GetTimerElapsed(StaggerTimerHandleTwo))
		if (SpawnDelay <= 0)
		{
			// Currently spawns them inside the spawner, this might be a problem
			ConstantSpawner();
			SpawnDelay = SpawnDelayFirst;
		}
		//UE_LOG(LogTemp, Warning, TEXT("x: %f"), SpawnDelay)
		else if (!StaggeredSpawns)
		{
			SpawnDelay -= DeltaTime;
		}
		if (StaggeredSpawns)
		{
			BPStaggered();
		}
	}
	else if (SwitchActivator)
	{
		// WeightTrigger sends a message to ActorSpawner which changes a bool SpawnNow in ActorSpawner to true
		Trapped();
	}
}

void AActorSpawner::ConstantSpawner()
{
	GetWorld()->SpawnActor<AMovingActor>(SpawnedActor, RootLocation, GetActorRotation());
	for (int32 i{ 0 }; i < numMeshSpawn; i++)
	{
		FVector Location = MeshArray[i]->GetComponentLocation();
		GetWorld()->SpawnActor<AMovingActor>(SpawnedActor, Location, GetActorRotation());
	}
}

// Atm these could use some restructuring to make it easier to change the stagger style
void AActorSpawner::StaggeredSpawningOne()
{
	GetWorld()->SpawnActor<AMovingActor>(SpawnedActor, RootLocation, GetActorRotation());

	if (numMeshSpawn >= 1)
	{
		for (int32 i{ 1 }; i < numMeshSpawn; i += 2)
		{
			FVector Location = MeshArray[i]->GetComponentLocation();
			GetWorld()->SpawnActor<AMovingActor>(SpawnedActor, Location, GetActorRotation());
		}
	}
}

void AActorSpawner::StaggeredSpawningTwo()
{
	for (int32 i{ 0 }; i < numMeshSpawn; i += 2)
	{
		FVector Location = MeshArray[i]->GetComponentLocation();
		GetWorld()->SpawnActor<AMovingActor>(SpawnedActor, Location, GetActorRotation());
	}
}