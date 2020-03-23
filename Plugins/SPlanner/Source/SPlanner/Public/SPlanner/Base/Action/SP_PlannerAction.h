// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_PlannerAction.generated.h"

class USP_Action;

class USP_PlannerComponent;

/** Action used by planner to generate plan. */
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct SPLANNER_API FSP_PlannerAction
{
	GENERATED_BODY()

public:
	/** The handled action. */
	const USP_Action* Handle = nullptr;

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

	/**
	*	Number of time this Action has been added to the plan.
	*	Used for Handle->GenerationType == ESP_ActionGenerationType::AGT_RandomWeight.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	int TimesNumInPlan = 0;

	/** Create PlannerAction with random GeneratedWeight. */
	static FSP_PlannerAction Make(const USP_PlannerComponent* Planner, const USP_Action* Action, bool bInAchievePlan = false, bool bInIsForced = false);

	/** Create PlannerAction from an other with random GeneratedWeight. */
	static FSP_PlannerAction Make(const USP_PlannerComponent* Planner, const FSP_PlannerAction& Other);
};