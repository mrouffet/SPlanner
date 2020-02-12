#pragma once

#include "SP_PlanState.generated.h"

/** The current state of the plan. */
UENUM(BlueprintType, Category = "SPlanner|Planner")
enum class ESP_PlanState : uint8
{
	/** Can be executed. */
	PS_Valid					UMETA(DisplayName = "Valid"),

	/** Error occured while generating. */
	PS_Invalid					UMETA(DisplayName = "Invalid"),

	/** Being computing by planner. */
	PS_Computing				UMETA(DisplayName = "Computing"),

	/** In queue for computation by planner. */
	PS_WaitForCompute			UMETA(DisplayName = "WaitForCompute"),

	/** Finished with success planner. */
	PS_Finished					UMETA(DisplayName = "Finished"),

	/** Planner inactive (out of LOD range). */
	PS_Inactive					UMETA(DisplayName = "Inactive"),
};

/** The current state of the task executed. */
UENUM(BlueprintType, Category = "SPlanner|Planner")
enum class ESP_PlanExecutionState : uint8
{
	/** Active task, called each frame. */
	PES_Running			UMETA(DisplayName = "Running"),

	/** Task end with success. */
	PES_Succeed			UMETA(DisplayName = "Succeed"),

	/** Task end with failure. */
	PES_Failed			UMETA(DisplayName = "Failed"),
};