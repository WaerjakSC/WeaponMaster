// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class WEAPONMASTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input variable
	FVector2D MovementInput;
	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Dash();
	FVector2D DashDirection;
	
	UFUNCTION(BlueprintCallable)
		void PauseGame();

	// Stop dash and dashnow used in Dash() function
	void DashNow();
	UFUNCTION(BlueprintCallable)
	void StopDash();
	void DashImplementation();
	// StartTimer() sends an event to blueprint which sets Dashing to false after DashDuration
	UFUNCTION(BlueprintImplementableEvent)
		void StartTimer();

	// Duration of the dash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
		float DashDuration = .2f;
	// Sets the speed used for dash
	UPROPERTY(EditAnywhere, Category = "Dash")
		float DashSpeed = 25000.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setup")
		UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Camera Setup")
		USpringArmComponent* CameraArm;


	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);
	/*OnHit function is called when the Player hits something*/
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	float WalkSpeed{ 750.f };
	bool DelayedDash;
	bool Dashing;
	float DashTimer{ 0.f };
	FVector NewLoc;
	FVector CurrentLoc;
	FVector DashStart;
	FVector DashEnd;
	
};
