#pragma once

#include <SPlanner/Miscs/SP_Weight.h>

#include "SP_POIAction.generated.h"

class USP_AITask;
class USP_Goal;

/**
*	Planner POI Action.
*	Pair task and weight.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_POIAction
{
	GENERATED_BODY()

protected:
	/** The handled Task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_AITask* Task = nullptr;

	/** All goals served by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> ServedGoals;

	/** All goals achieved by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> AchievedGoals;

public:
	/**
	*	The default weight of the action.
	*	Increase Weight to increase chances of adding the action to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FSP_Weight Weight;

	/** Getter of Task. */
	USP_AITask* GetTask() const;

	/** Getter of ServedGoals. */
	const TArray<USP_Goal*>& GetServedGoals() const;

	/** Getter of AchievedGoals. */
	const TArray<USP_Goal*>& GetAchievedGoals() const;
};