#pragma once

#include <Engine/DeveloperSettings.h>
#include "Settings.generated.h"

#if WITH_EDITORONLY_DATA
/**
*	Debug plan enum flags.
*/
UENUM(Meta = (Bitflags))
enum class ESP_DebugFlag : uint8
{
	/** Log move list and generated plan. */
	PD_Plan				UMETA(DisplayName = "Plan"),

	/** Log task execution (1 time). */
	PD_TaskExecute		UMETA(DisplayName = "Execute"),

	/** Log task execution frames. */
	PD_TaskTick			UMETA(DisplayName = "Task"),
};

ENUM_CLASS_FLAGS(ESP_DebugFlag);
#endif

/**
 *	SPlanner static settings.
 */
UCLASS(Config = Game, defaultconfig, DisplayName = "SPlanner")
class SPLANNER_API USP_Settings : public UDeveloperSettings
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	/** Debug log flags for planner. */
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "ESP_DebugFlag"), Category = "Debug")
	uint8 DebugMask = 0u;

	/** Display time of planner logs. */
	UPROPERTY(EditAnywhere, Category = "Debug")
	float DebugScreenDisplayTime = 10.0f;

	/** The base log key for log. */
	UPROPERTY(EditAnywhere, Category = "Debug")
	int DebugLogKey = 12158;
#endif

	/**
	*	The maximum planner depth while building a plan.
	*	Plan generation complexity: O(!n), n = MaxPlannerDepth.
	*/
	UPROPERTY(Config, EditAnywhere)
	uint8 MaxPlannerDepth = 5u;

public:
#if WITH_EDITOR
	/** Static accessor of DebugMask. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static uint8 GetDebugMask();

	/** Static accessor of DebugScreenDisplayTime. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static float GetDebugScreenDisplayTime();

	/** Static accessor of MoveListLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static int GetMoveListLogKey();

	/** Static accessor of PlanLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static int GetPlanLogKey();

	/** Static accessor of TaskExecuteLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static int GetTaskExecuteLogKey();

	/** Static accessor to increment TaskExecuteLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static void IncrementTaskExecuteLogKey();

	/** Static accessor to reset TaskExecuteLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Settings")
	static void ResetTaskExecuteLogKey();
#endif

	/** Static accessor of MaxPlannerDepth */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Settings")
	static uint8 GetMaxPlannerDepth();
};