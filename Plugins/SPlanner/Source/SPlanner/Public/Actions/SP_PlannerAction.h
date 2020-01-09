#pragma once

#include <Actions/SP_Action.h>
#include "SP_PlannerAction.generated.h"

/** Action used by planner to generate plan. */
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct FSP_PlannerAction : public FSP_Action
{
	GENERATED_BODY()

	/** Achieve current plan (ie: EndActions). */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	bool bAchievePlan = false;

	FSP_PlannerAction() = default;
	FSP_PlannerAction(USP_ActionStep* InStep, float InWeight, bool bInAchievePlan = false);
};