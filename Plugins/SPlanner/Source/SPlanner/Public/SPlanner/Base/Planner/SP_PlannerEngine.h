// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Action/SP_PlannerActionSet.h>

class USP_ActionImpl;
class USP_PlannerComponent;
class USP_PlanGenInfos;

/** Planner algorithms implementation. */
class SPLANNER_API SP_PlannerEngine
{
	/** Internal implementation of LinearConstruct. */
	static bool LinearConstruct_Internal(USP_PlanGenInfos* Infos, uint8 CurrDepth);

	/**
	*	Update planner action set using action generation type.
	*	Return the action key requiered to rollback update.
	*/
	static uint64 UpdateSet(USP_PlanGenInfos* Infos, const FSP_PlannerAction& Action);

	/**
	*	Rollback previous planner action set update using saved key.
	*/
	static void RollbackSet(USP_PlanGenInfos* Infos, const FSP_PlannerAction& Action, uint64 ActionKey);

public:
	/** Planning linear plan construction. */
	static bool LinearConstruct(USP_PlanGenInfos* Infos);
};