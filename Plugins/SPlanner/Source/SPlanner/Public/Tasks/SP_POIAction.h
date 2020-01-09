#pragma once

#include "SP_POIAction.generated.h"

class USP_Task;
class USP_Goal;

/**
*	Planner POI Action.
*	Pair task and weight.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_POIAction
{
	GENERATED_BODY()

	/** The handled task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_Task* Task;

	/**
	*	The weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float Weight = 1.0f;

	/** All goals served by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> ServedGoals;

	/** All goals achieved by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> AchievedGoals;
};