// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Engine/Engine.h"

/* TODO: 
Dashing towards a wall puts you inside it if you're next to it already
Set up fake enemies with health to test the dashing damage system
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
	// Configure character dash settings
	GetCharacterMovement()->MaxFlySpeed = 25000.f;
	GetCharacterMovement()->MaxAcceleration = 25000.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 15000.f;

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
			// Might keep going
			SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), NewLoc, DeltaTime, 5000.f));
			FVector DashX = NewLoc - GetActorLocation();
			float DashEnd = FMath::Abs(DashX.X) + FMath::Abs(DashX.Y);
			//UE_LOG(LogTemp, Warning, TEXT("Distance from EndPoint: %f"), DashEnd)
			if (DashEnd <= 20.f)
			{
				Dashing = false;
			}
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
		//FHitResult Hit;
		//bool HitResult = false;
		//// GameTraceChannel1 = Ground custom channel in editor
		//HitResult = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, Hit);
		//if (HitResult)
		//{
		//	//Updates cursor
		//	FVector CursorFV = Hit.ImpactNormal;
		//	FRotator CursorR = CursorFV.Rotation();

		//	//Set the new direction of the pawn:
		//	FVector CursorLocation = Hit.Location;
		//	//Set Z to a little above ground
		//	FVector TempLocation = FVector(CursorLocation.X, CursorLocation.Y, 30.f);

		//	//Pure vector math
		//	FVector NewDirection = TempLocation - GetActorLocation();
		//	NewDirection.Z = 0.f;
		//	NewDirection.Normalize();
		//	// Remove when done debugging
		//	DrawDebugLine(GetWorld(), GetActorLocation(), TempLocation, FColor(0, 0, 255), false, 0.f, 0.f, 10.f);
		//	//SetActorRotation(NewDirection.Rotation());
		//}
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
	DashDirection.X = MovementInput.X;
	DashDirection.Y = MovementInput.Y;
	FVector EndPoint = (DashLength / DashDirection.Size()) * DashDirection;
	NewLoc = GetActorLocation() + EndPoint;
	UE_LOG(LogTemp, Warning, TEXT("EndPoint.X: %f, EndPoint.Y: %f"), EndPoint.X, EndPoint.Y)
	Dashing = true;
	//StartTimer();
}

void APlayerCharacter::StopDash()
{
	Dashing = false;
	NewLoc = GetActorLocation();
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

}

void APlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopDash();
	Dashing = false;
}

