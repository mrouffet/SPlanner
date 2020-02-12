// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Debug/SP_DebugMaco.h>
#include <SPlanner/Debug/SP_DebugVerbosity.h>
#include <SPlanner/Debug/SP_DebugBenchmark.h>

#include <UObject/Object.h>
#include "SP_Debug.generated.h"

UCLASS(Abstract, ClassGroup = "SPlanner|Debug")
class SPLANNER_API USP_Debug : public UObject
{
	GENERATED_BODY()

public:
	/** Blueprint accessor to SP_LOG */
	UFUNCTION(BlueprintCallable, DisplayName = "SP_Log", Category = "SPlanner|Debug")
	static void Log(UObject* Caller, const FString& Str = "Hello", ESP_DebugVerbosity Verbosity = ESP_DebugVerbosity::DV_Display);

	/**
	*	Blueprint accessor to SP_LOG_SCREEN.
	*	Use Verbosity color.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "SP_LogScreen", Category = "SPlanner|Debug")
	static void LogScreen(UObject* Caller, const FString& Str = "Hello", ESP_DebugVerbosity Verbosity = ESP_DebugVerbosity::DV_Display);

	/**
	*	Blueprint accessor to SP_LOG_SCREEN.
	*	Use custom color.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "SP_LogScreenFull", Category = "SPlanner|Debug")
	static void LogScreenFull(UObject* Caller, const FString& Str = "Hello", ESP_DebugVerbosity Verbosity = ESP_DebugVerbosity::DV_Display, FLinearColor Color = FLinearColor::Yellow, float Time = 5.0f);
};