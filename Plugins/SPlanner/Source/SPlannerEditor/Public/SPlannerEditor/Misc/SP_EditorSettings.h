// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Engine/DeveloperSettings.h>
#include "SP_EditorSettings.generated.h"

/** Debug plan enum flags. */
UENUM(Meta = (Bitflags), Category = "SPlannerEditor")
enum class ESP_EditorDebugFlag : uint8
{
	/** Log move list and generated plan. */
	ED_Plan				UMETA(DisplayName = "Plan"),

	/** Log task execution (1 time). */
	ED_TaskExecute		UMETA(DisplayName = "Execute"),

	/** Log task execution frames. */
	ED_TaskTick			UMETA(DisplayName = "Task"),

	/** Log task execution frames. */
	ED_Formation		UMETA(DisplayName = "Formation"),
};

ENUM_CLASS_FLAGS(ESP_EditorDebugFlag);

/**
 *	SPlanner editor static settings.
 *	These settings are locally saved in Intermediate/
 */
UCLASS(Config = Editor, DisplayName = "SPlanner", Category = "SPlannerEditor")
class SPLANNEREDITOR_API USP_EditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

protected:
	/** Debug log flags for planner. */
	UPROPERTY(Config, EditAnywhere, meta = (Bitmask, BitmaskEnum = "ESP_EditorDebugFlag"), Category = "Debug")
	uint8 DebugMask = 0u;

	/** Display time of planner logs. */
	UPROPERTY(Config = Game, EditAnywhere, Category = "Debug")
	float DebugScreenDisplayTime = 10.0f;

	/** The base log key for log. */
	UPROPERTY(Config = Game, EditAnywhere, Category = "Debug")
	int DebugLogKey = 12158;

public:
	/** Static accessor of DebugMask. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static uint8 GetDebugMask();

	/** Static accessor of DebugScreenDisplayTime. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static float GetDebugScreenDisplayTime();

	/** Static accessor of MoveListLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static int GetActionListLogKey();

	/** Static accessor of PlanLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static int GetPlanLogKey();

	/** Static accessor of PlanLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static int GetPlanGenerationLogKey();

	/** Static accessor of TaskExecuteLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static int GetTaskExecuteLogKey();

	/** Static accessor to increment TaskExecuteLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static void IncrementTaskExecuteLogKey();

	/** Static accessor to reset TaskExecuteLogKey. */
	UFUNCTION(BlueprintCallable, Category = "SPlannerEditor|Settings")
	static void ResetTaskExecuteLogKey();
};