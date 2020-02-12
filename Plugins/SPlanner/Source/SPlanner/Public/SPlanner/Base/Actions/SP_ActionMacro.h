// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

/**
*	Helper macro to call parent pre-condition implementation.
*	Should always be called in children PreCondition_Implementation override.
*/
#define SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags, ...)\
	if (!Super::PreCondition(Planner, GeneratedPlan, PlannerFlags, ##__VA_ARGS__))\
		return false;


/**
*	Helper macro to call parent post-condition implementation.
*	Should always be called in children PostCondition_Implementation override.
*/
#define SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags, ...)\
	PlannerFlags = Super::PostCondition(Planner, PlannerFlags, ##__VA_ARGS__);