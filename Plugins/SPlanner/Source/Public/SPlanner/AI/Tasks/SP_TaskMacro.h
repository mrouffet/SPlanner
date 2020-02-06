#pragma once

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Miscs/SP_Settings.h>
#include <SPlanner/Miscs/SP_FlagHelper.h>


/**
*	Helper macro to call Begin() parent implementation.
*	Should always be called in children Begin() override.
*/
#define SP_TASK_BEGIN_SUPER(...)\
	if (!Super::Begin(__VA_ARGS__))\
		return false;

/**
*	Helper macro to call Tick() parent implementation.
*	Should always be called in children Tick() override.
*/
#define SP_TASK_TICK_SUPER(...)\
	ESP_PlanExecutionState SuperState = Super::Tick(__VA_ARGS__);\
	if (SuperState != ESP_PlanExecutionState::PES_Succeed)\
		return SuperState;

/**
*	Helper macro to call End() parent implementation.
*	Should always be called in children End() override.
*/
#define SP_TASK_END_SUPER(...)\
	if (!Super::End(__VA_ARGS__))\
		return false;

/**
*	Helper macro to call Cancel() parent implementation.
*	Should always be called in children Cancel() override.
*/
#define SP_TASK_CANCEL_SUPER(...)\
	if (!Super::Cancel(__VA_ARGS__))\
		return false;


#if SP_DEBUG_EDITOR

/**
*	Helper macro to check whether task execute debug is shown.
*/
#define SP_IF_TASK_EXECUTE(CheckSelected)\
	if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_TaskExecute) && CheckSelected.IsSelectedInEditor())

/**
*	Helper macro to check whether task execute debug is shown.
*/
#define SP_IF_TASK_TICK(CheckSelected)\
	if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_TaskTick) && CheckSelected.IsSelectedInEditor())


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