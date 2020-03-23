// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Action/SP_ActionSet.h>
#include <SPlanner/Base/Action/SP_PlannerAction.h>

#include "SP_PlannerActionSet.generated.h"

// Sort using weight.
struct SPLANNER_API FSP_PlannerActionSortFunctor
{
	// Decreasing weight sort.
	bool operator()(const FSP_PlannerAction& lhs, const FSP_PlannerAction& rhs) const
	{
		return lhs.GeneratedWeight > rhs.GeneratedWeight;
	}
};

/**
*	Shuffled action using random and weights.
*/
USTRUCT(BlueprintType, Category = "SPlanner|Action")
struct SPLANNER_API FSP_PlannerActionSet
{
	GENERATED_BODY()

	/**
	*	Begin actions.
	*	Only one of these will be selected by the planner.
	*/
	TArray<FSP_PlannerAction> BeginActions;

	/**
	*	All possible actions (include core, forced and end).
	*	The planner will end the generation while a end action will be added.
	*/
	TArray<FSP_PlannerAction> Actions;

	/**
	*	Whether this action set contain forced actions.
	*	While true, perform additionnal check to ensure every forced action has been added to the plan.
	*/
	bool bHasForcedActions = false;

	FSP_PlannerActionSet() = default;

	/** Getter of Action array depending on Depth. */
	TArray<FSP_PlannerAction>& GetActionsFromDepth(uint8 Depth);
	
	/** const Getter of Action array depending on Depth. */
	const TArray<FSP_PlannerAction>& GetActionsFromDepth(uint8 Depth) const;

	/**
	*	Create PlannerActionSet from action set.
	*	Shuffle every actions using random and weights.
	*/
	static FSP_PlannerActionSet Make(const USP_PlannerComponent* Planner, const USP_ActionSet* ActionSet, bool* bCanBeAchievedPtr = nullptr);
};