

#include "PickUp.h"

// Sets default values
APickUp::APickUp()
{
	PotatoCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = PotatoCollider;
	PotatoCollider->InitBoxExtent(FVector(64.f, 64.f, 64.f));
	PotatoCollider->bGenerateOverlapEvents = true;

	PotatoVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Representation"));
	PotatoVisual->SetCollisionProfileName("NoCollision");
	PotatoVisual->SetupAttachment(RootComponent);
	PotatoVisual->bGenerateOverlapEvents = false;

	
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	RotateActor();
	StartPoint = Crystal->GetActorLocation();
	PotatoCollider->OnComponentBeginOverlap.AddDynamic(this, &APickUp::OnOverlap);
}


void APickUp::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, 
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult &SweepResult)
{
	/*if (OtherActor->IsA(AProtoChar::StaticClass()))
	{
		FVector CurLoc = OtherActor->GetActorLocation();
		Cast<ACometGameMode>(GetWorld()->GetAuthGameMode())->RespawnLocation = CurLoc;
		Cast<ACometGameMode>(GetWorld()->GetAuthGameMode())->NumActivated++;
		StartMovement();
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}*/
}



