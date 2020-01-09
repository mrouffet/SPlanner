#pragma once

#include <Actions/SP_ActionBase.h>
#include "SP_Action.generated.h"

/**
*	Action Base with Weight used to shuffle actions before plan generation.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_Action : public FSP_ActionBase
{
	GENERATED_BODY()

	/**
	*	The weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float Weight = 1.0f;

	FSP_Action() = default;
	FSP_Action(USP_ActionStep* InStep, float InWeight);
};