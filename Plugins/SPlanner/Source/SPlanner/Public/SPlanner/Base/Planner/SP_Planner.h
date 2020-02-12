// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Actions/SP_PlannerActionSet.h>

class USP_ActionStep;
class USP_PlannerComponent;

/** Planner algorithms implementation. */
class SPLANNER_API SP_Planner
{
public:
	/**
	*	Struct infos for linear generation.
	*	Planner can be nullptr for custom plan generation call.
	*/
	struct FSP_LinearConstructInfos
	{
		const USP_PlannerComponent& Planner;
		FSP_PlannerActionSet& PlannerActions;
		TArray<USP_ActionStep*>& OutPlan;
		uint8 MaxDepth = 7u;
		float LODLevel = -1.0f;
	};

	/** Planning linear plan construction. */
	static bool LinearConstruct(FSP_LinearConstructInfos Infos);

private:
	/** Internal implementation of LinearConstruct. */
	static bool LinearConstruct_Internal(FSP_LinearConstructInfos& Infos, uint8 CurrDepth, uint64 PlannerFlags);

	/**
	*	Update planner action set using action generation type.
	*	Return the action key requiered to rollback update.
	*/
	static uint64 UpdateSet(FSP_LinearConstructInfos& Infos, const FSP_PlannerAction& Action);

	/**
	*	Rollback previous planner action set update using saved key.
	*/
	static void RollbackSet(FSP_LinearConstructInfos& Infos, const FSP_PlannerAction& Action, uint64 ActionKey);
};