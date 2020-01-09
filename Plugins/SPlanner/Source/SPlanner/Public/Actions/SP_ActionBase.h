#pragma once

#include <Miscs/Flags/SP_PlannerFlags.h>

#include "SP_ActionBase.generated.h"

class USP_ActionStep;

class USP_PlannerComponent;

/**
*	One or more ActionStep used by Planner to generate a plan.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_ActionBase
{
	GENERATED_BODY()

	/** The handled action step. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_ActionStep* Step = nullptr;

	FSP_ActionBase() = default;
	FSP_ActionBase(USP_ActionStep* InStep);


#if WITH_EDITOR
	/**
	*	Debug string for plan logging.
	*	Step asset name.
	*/
	FString GetDebugStr() const;
#endif
};