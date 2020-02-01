// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Miscs/SP_FlagHelper.h>


/**
*	Helper macro to call Begin() parent implementation.
*	Should always be called in children Begin() override.
*/
#define SP_TASK_SUPER_BEGIN(...)\
	if (!Super::Begin_Internal_Implementation(__VA_ARGS__))\
		return false;

/**
*	Helper macro to call Tick() parent implementation.
*	Should always be called in children Tick() override.
*/
#define SP_TASK_SUPER_TICK(...)\
	ESP_PlanExecutionState SuperState = Super::Tick_Internal_Implementation(__VA_ARGS__);\
	if (SuperState != ESP_PlanExecutionState::PES_Succeed)\
		return SuperState;

/**
*	Helper macro to call End() parent implementation.
*	Should always be called in children End() override.
*/
#define SP_TASK_SUPER_END(...)\
	if (!Super::End_Internal_Implementation(__VA_ARGS__))\
		return false;

#if SP_DEBUG_EDITOR

#include <SPlannerEditor/Miscs/SP_EditorSettings.h>

/**
*	Helper macro to check whether task execute debug is shown.
*/
#define SP_IF_TASK_EXECUTE(CheckSelected)\
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_TaskExecute) && CheckSelected->IsSelectedInEditor())

/**
*	Helper macro to check whether task execute debug is shown.
*/
#define SP_IF_TASK_TICK(CheckSelected)\
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_TaskTick) && CheckSelected->IsSelectedInEditor())


/**
*	Helper macro to log task execute.
*/
#define SP_LOG_TASK_EXECUTE(CheckSelected, Str, ...)\
	SP_IF_TASK_EXECUTE(CheckSelected)\
	SP_LOG_SCREEN_FULL(Display, USP_EditorSettings::GetTaskExecuteLogKey(), FColor::Green,  USP_EditorSettings::GetDebugScreenDisplayTime(), "%s: " Str, *GetName(), ##__VA_ARGS__)

/**
*	Helper macro to log all task ticks.
*/
#define SP_LOG_TASK_TICK(CheckSelected, Str, ...)\
	SP_IF_TASK_TICK(CheckSelected)\
	SP_LOG_SCREEN_FULL(Display, -1, FColor::Magenta,  USP_EditorSettings::GetDebugScreenDisplayTime(), "%s: " Str, *GetName(), ##__VA_ARGS__)

#else

#define SP_IF_TASK_EXECUTE(...)
#define SP_IF_TASK_TICK(...)

#define SP_LOG_TASK_EXECUTE(...)
#define SP_LOG_TASK_TICK(...)

#endif