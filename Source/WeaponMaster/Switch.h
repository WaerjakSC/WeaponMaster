

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Switch.generated.h"

UCLASS()
class WEAPONMASTER_API ASwitch : public AActor
{
	GENERATED_BODY()

public:
	// Callable function for a Use system
	UFUNCTION(BlueprintImplementableEvent)
		bool OnUsed(ACharacter* character);
	// Toggles between switch states
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Status")
		bool OnOff;
	/*
	Getter for bool OnOff
	*/
	bool GetOnOff();

	/*UFUNCTION(BlueprintImplementableEvent)
		bool StartFocusItem();
	UFUNCTION(BlueprintImplementableEvent)
		bool EndFocusItem();*/

};
