// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Actions/SP_Action.h>
#include "SP_PlannerAction.generated.h"

/** Action used by planner to generate plan. */
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct SPLANNER_API FSP_PlannerAction
{
	GENERATED_BODY()

public:
	/** The handled action. */
	const FSP_Action* Handle = nullptr;

	/**
	*	The generated weight of the action.
	*	Increase Weight to increase chances of adding the task to the plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float GeneratedWeight = -1.0f;

	/** Achieve current plan (ie: EndActions). */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	bool bAchievePlan = false;

	/** Achieve current plan (ie: EndActions). */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	bool bIsForced = false;

	/** Create PlannerAction with random GeneratedWeight. */
	static FSP_PlannerAction Make(const FSP_Action* Action, float LODLevel, bool bInAchievePlan = false, bool bInIsForced = false);
};