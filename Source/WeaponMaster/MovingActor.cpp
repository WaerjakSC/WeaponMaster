

#include "MovingActor.h"


// Sets default values
AMovingActor::AMovingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Moving Actor Collision"));
	RootComponent = ProjectileCollision;
	ProjectileCollision->bGenerateOverlapEvents = true;

	ProjectileMoveVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Representation"));
	ProjectileMoveVisual->SetCollisionProfileName("NoCollision");
	ProjectileMoveVisual->SetupAttachment(RootComponent);
	ProjectileMoveVisual->bGenerateOverlapEvents = false;

	ShootyShoot = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	InitialLifeSpan = 3;

}

// Called when the game starts or when spawned
void AMovingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovingActor::DestroyItem()
{
	this->Destroy();
}

