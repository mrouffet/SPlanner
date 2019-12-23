#pragma once

#include <GameFramework/Character.h>
#include "PlannerCharacter.generated.h"

class USP_PlannerComponent;
class USP_TargetComponent;

/**
*	SPlanner character implementation.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Character")
class SPLANNER_API ASP_PlannerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Planning behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Planner")
	USP_PlannerComponent* Planner = nullptr;

	/** Target behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Planner")
	USP_TargetComponent* Target = nullptr;

public:
	ASP_PlannerCharacter(const FObjectInitializer& ObjectInitializer);
};