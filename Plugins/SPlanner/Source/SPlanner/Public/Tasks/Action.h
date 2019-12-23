#pragma once

#include "Action.generated.h"

class USP_Task;

/**
*	Planner Action.
*	Pair task and weight.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_Action
{
	GENERATED_BODY()

	/** The handled task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Action")
	USP_Task* Task;

	/**
	*	The weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Action")
	float Weight = 1.0f;

	/** Whether this task achieve the current goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Action")
	bool bAchieveGoal = false;

	FSP_Action() = default;
	FSP_Action(USP_Task* InTask, float InWeight, bool InbAchieveGoal);
	FSP_Action(const FSP_Action& Other, float WeightMultiplier);
};