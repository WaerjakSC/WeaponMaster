
#include "WeightTrigger.h"


// Sets default values
AWeightTrigger::AWeightTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Weight Check"));
	SphereTrigger->InitSphereRadius(76.f);
	RootComponent = SphereTrigger;
	SphereTrigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	
	SphereTrigger->bGenerateOverlapEvents = true;
	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWeightTrigger::OnOverlap);
	SphereTrigger->OnComponentEndOverlap.AddDynamic(this, &AWeightTrigger::OnOverlapEnd);


}

// Called when the game starts or when spawned
void AWeightTrigger::BeginPlay()
{
	Super::BeginPlay();
	StartPos = GetActorLocation();
}

// Called every frame
void AWeightTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeightTrigger::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	//if (OtherActor->IsA(AProtoChar::StaticClass()))
	//{
	//	OverlappedPlayer();
	//}
}

void AWeightTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
void AWeightTrigger::HidePlatform()
{
	if (bHidden == false)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}
}