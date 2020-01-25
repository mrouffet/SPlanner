#pragma once

#include <SPlanner/Actions/SP_ActionBase.h>
#include "SP_PlannerAction.generated.h"

/** Action used by planner to generate plan. */
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_PlannerAction : public FSP_ActionBase
{
	GENERATED_BODY()

	/**
	*	The weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float Weight = 1.0f;

	/** Achieve current plan (ie: EndActions). */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	bool bAchievePlan = false;

	/**
	*	Has been already added to the plan.
	*	This is the only value that can be changed during plan generation.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	mutable bool bIsInPlan = false;

	FSP_PlannerAction() = default;
	FSP_PlannerAction(USP_ActionStep* InStep, float InWeight, bool bInAchievePlan = false);
};