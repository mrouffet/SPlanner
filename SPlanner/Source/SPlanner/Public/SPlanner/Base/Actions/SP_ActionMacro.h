// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

/**
*	Helper macro to call parent pre-condition implementation.
*	Should always be called in children PreCondition_Implementation override.
*/
#define SP_ACTION_STEP_SUPER_PRECONDITION(...)\
	if (!Super::PreCondition_Implementation(__VA_ARGS__))\
		return false;


/**
*	Helper macro to call parent post-condition implementation.
*	Should always be called in children PostCondition_Implementation override.
*/
#define SP_ACTION_STEP_SUPER_POSTCONDITION(...)\
	if (!Super::PostCondition_Implementation(__VA_ARGS__))\
		return false;

/**
*	Helper macro to call parent reset post-condition implementation.
*	Should always be called in children PostCondition_Implementation override.
*/
#define SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(...)\
	if (!Super::ResetPostCondition_Implementation(__VA_ARGS__))\
		return false;