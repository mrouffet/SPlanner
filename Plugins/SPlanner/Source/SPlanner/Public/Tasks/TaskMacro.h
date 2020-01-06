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



#if SP_DEBUG

#if WITH_EDITOR

/**
*	Helper macro to check whether task execute debug is shown.
*/
#define SP_IF_TASK_EXECUTE(CheckSelected)\
	if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_TaskExecute) && CheckSelected->IsSelectedInEditor())

/**
*	Helper macro to check whether task execute debug is shown.
*/
#define SP_IF_TASK_TICK(CheckSelected)\
	if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_TaskTick) && CheckSelected->IsSelectedInEditor())

#else

#define SP_IF_TASK_EXECUTE(...)
#define SP_IF_TASK_TICK(...)

#endif

/**
*	Helper macro to log task execute.
*/
#define SP_LOG_TASK_EXECUTE(CheckSelected, Str, ...)\
	SP_IF_TASK_EXECUTE(CheckSelected)\
	SP_LOG_SCREEN_FULL(Display, USP_Settings::GetTaskExecuteLogKey(), FColor::Green,  USP_Settings::GetDebugScreenDisplayTime(), "%s: " Str, *GetName(), ##__VA_ARGS__)


/**
*	Helper macro to log all task ticks.
*/
#define SP_LOG_TASK_TICK(CheckSelected, Str, ...)\
	SP_IF_TASK_TICK(CheckSelected)\
	SP_LOG_SCREEN_FULL(Display, -1, FColor::Magenta,  USP_Settings::GetDebugScreenDisplayTime(), "%s: " Str, *GetName(), ##__VA_ARGS__)

#else

#define SP_IF_TASK_EXECUTE(...)
#define SP_IF_TASK_TICK(...)

#define SP_LOG_TASK_EXECUTE(...)
#define SP_LOG_TASK_TICK(...)

#endif