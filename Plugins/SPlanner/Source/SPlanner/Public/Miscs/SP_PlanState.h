#pragma once

#include "SP_PlanState.generated.h"

/** The current state of the plan. */
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Misc")
enum class ESP_PlanState : uint8
{
	/** Can be executed. */
	PS_Valid					UMETA(Display = "Valid"),

	/** Error occured while generating. */
	PS_Invalid					UMETA(Display = "Invalid"),

	/** Being computing by planner. */
	PS_Computing				UMETA(Display = "Computing"),

	/** In queue for computation by planner. */
	PS_WaitForCompute			UMETA(Display = "WaitForCompute"),

	/** Finished with success planner. */
	PS_Finished					UMETA(Display = "Finished"),
};

/** The current state of the task executed. */
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Misc")
enum class ESP_PlanExecutionState : uint8
{
	/** Active task, called each frame. */
	PES_Running			UMETA(Display = "Running"),

	/** Task end with success. */
	PES_Succeed			UMETA(Display = "Succeed"),

	/** Task end with failure. */
	PES_Failed			UMETA(Display = "Failed"),
};