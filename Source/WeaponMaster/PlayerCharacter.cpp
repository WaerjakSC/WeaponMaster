// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Engine/Engine.h"

/* TODO: 
Set player character to block enemies and change it during dash to overlap instead, do the same to enemies for the player.
Set up fake enemies with health to test the dashing damage system
Use linetrace to end of dash and at start of dash function change collision of all hit by linetrace to overlap player instead of blocking, and do the same to player
Or create an invisible object line that "erupts" at the end of the dash, causing damage to anything it's overlapped with (this would work well for more irregular area of effect type damage)
How is animation tied into things like area of effect attacks? Is the animation itself responsible for triggering damage done? (I think so)
*/
// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ACharacter::GetCapsuleComponent()->InitCapsuleSize(30.f, 100.f);
	ACharacter::GetCapsuleComponent()->bGenerateOverlapEvents = true;

	ACharacter::GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);
	ACharacter::GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHit);


	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Rotate to direction of movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	ACharacter::GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ACharacter::GetMesh()->SetRelativeScale3D(FVector(12.75f, 12.75f, 12.75f));

	// Sets up camera boom and camera
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraArm->bInheritYaw = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// This only works in BeginPlay for some reason
	ACharacter::GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHit);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Controller && Controller->IsLocalController())
	{
		if (Dashing)
		{
			DashImplementation();
		}
		// If no movement input is detected, accept the next movement input within 150ms as the next dash direction
		if (DelayedDash)
		{
			DashTimer += DeltaTime;
			if (DashTimer >= .15f)
			{
				DashNow();
				DashTimer = 0.f;
				DelayedDash = false;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);
	FInputActionBinding& toggle = InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::PauseGame);
	toggle.bExecuteWhenPaused = true; // Even if the game is paused, catch this event!
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if (!Dashing)
	{
		MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
		AddMovementInput(FVector(1.f, 0.f, 0.f), MovementInput.X);
	}
}
void APlayerCharacter::MoveRight(float AxisValue)
{
	if (!Dashing)
	{
		MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
		AddMovementInput(FVector(0.f, 1.f, 0.f), MovementInput.Y);
	}
}
void APlayerCharacter::Dash()
{
	if (!Dashing)
	{
		if (MovementInput.Size() != 0.f)
		{
			DashNow();
		}
		else
		{
			DelayedDash = true;
		}
	}
}
void APlayerCharacter::DashNow()
{
	Dashing = true;
	// DashStart = GetActorLocation();
	DashDirection = MovementInput;
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	GetCharacterMovement()->MaxAcceleration = DashSpeed;
	StartTimer();
	UE_LOG(LogTemp, Warning, TEXT("DashDirection.X = %f, DashDirection.Y = %f"), DashDirection.X, DashDirection.Y)
} 
void APlayerCharacter::StopDash()
{
	Dashing = false;
	// Temp to visualize dash distance
	// DashEnd = GetActorLocation();
	// DrawDebugLine(GetWorld(), DashStart, DashEnd, FColor(0, 0, 255), true, 0.f, 0.f, 40.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxAcceleration = 2048.f;
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
}
void APlayerCharacter::DashImplementation()
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), DashDirection.X);
	AddMovementInput(FVector(0.f, 1.f, 0.f), DashDirection.Y);
}
void APlayerCharacter::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}
void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	// For enemies, when dashing should be the only time player is capable of overlapping
	// So OnOverlap should trigger a damage function for the enemy
}

void APlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Dash should only trigger a block if hitting something like a wall
	StopDash();
}

