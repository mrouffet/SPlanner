#pragma once

#include <Curves/CurveFloat.h>

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

protected:
	/** The handled Task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_Task* Task = nullptr;

	/**
	*	The default weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float DefaultWeight = 1.0f;

	/**
	*	The weight of the action depending on the LOD level.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	FRuntimeFloatCurve WeightLODCurve;

	/** All goals served by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> ServedGoals;

	/** All goals achieved by this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_Goal*> AchievedGoals;

public:
	/** Getter of Task. */
	USP_Task* GetTask() const;

	/** Getter of Weight using WeightCurve with valid LODLevel, otherwise DefaultWeight. */
	float GetWeight(float LODLevel = -1.0f) const;

	/** Getter of ServedGoals. */
	const TArray<USP_Goal*>& GetServedGoals() const;

	/** Getter of AchievedGoals. */
	const TArray<USP_Goal*>& GetAchievedGoals() const;
};