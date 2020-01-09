#pragma once

#include <GameFramework/Character.h>
#include "SP_Character.generated.h"

class USP_ActionSetComponent;
class USP_TargetComponent;
class USP_InteractZoneComponent;

/**
*	SPlanner character implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Character")
class SPLANNER_API ASP_Character : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Action set used by Planner. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_ActionSetComponent* ActionSet = nullptr;

	/** Target behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_TargetComponent* Target = nullptr;

	/** POI Interact zone behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_InteractZoneComponent* InteractZone = nullptr;

public:
	ASP_Character(const FObjectInitializer& ObjectInitializer);
};