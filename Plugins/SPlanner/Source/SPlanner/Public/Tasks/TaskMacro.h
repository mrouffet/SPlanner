#pragma once

#include <Debug/Debug.h>
#include <Miscs/FlagHelper.h>

#define SP_TASK_BLUEPRINT_IMPLEMENTABLE 1

/**
*	Helper macro to call parent pre condition implementation.
*	Should always be called in children PreCondition_Implementation override.
*/
#define SP_TASK_SUPER_PRECONDITION(Planner, PlannerFlags, ...)\
	if (!Super::PreCondition(Planner, PlannerFlags, ##__VA_ARGS__))\
		return false;


/**
*	Helper macro to call parent post condition implementation.
*	Should always be called in children PostCondition_Implementation override.
*/
#define SP_TASK_SUPER_POSTCONDITION(Planner, PlannerFlags, ...)\
	PlannerFlags = Super::PostCondition(Planner, PlannerFlags, ##__VA_ARGS__);


/**
*	Helper macro to call parent implementation.
*	Should always be called in children Execute_Implementation override.
*/
#define SP_TASK_SUPER(Implementation, ...)\
	ESP_PlanExecutionState SuperState = Super::Implementation(##__VA_ARGS__);\
	if (SuperState != ESP_PlanExecutionState::PES_Succeed)\
		return SuperState;


/**
*	Helper macro to log task execute.
*/
#if SP_DEBUG

#define SP_LOG_EXECUTE(CheckSelected, Str, ...)\
	if (IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Execute) && CheckSelected->IsSelectedInEditor())\
		SP_LOG_SCREEN_FULL(Display, USP_Settings::GetExecuteLogKey(), FColor::Green,  USP_Settings::GetDebugScreenDisplayTime(), "%s: " Str, *GetName(), ##__VA_ARGS__)

/**
*	Helper macro to log all task.
*/
#define SP_LOG_TASK(CheckSelected, Str, ...)\
	if (IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Task) && CheckSelected->IsSelectedInEditor())\
		SP_LOG_SCREEN_FULL(Display, -1.0f, FColor::Magenta,  USP_Settings::GetDebugScreenDisplayTime(), "%s: " Str, *GetName(), ##__VA_ARGS__)
#else

#define SP_LOG_EXECUTE(...)
#define SP_LOG_TASK(...)

#endif