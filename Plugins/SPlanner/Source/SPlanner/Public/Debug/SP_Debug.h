#pragma once

#include <Debug/SP_DebugVerbosity.h>
#include <Debug/SP_DebugMaco.h>

#include <UObject/Object.h>
#include "SP_Debug.generated.h"

UCLASS(Abstract, ClassGroup = "SPlanner|Debug")
class USP_Debug : public UObject
{
	GENERATED_BODY()

public:
	/** Blueprint accessor to SP_LOG */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Debug")
	static void Log(UObject* Caller, const FString& Str = "Hello", ESP_DebugVerbosity Verbosity = ESP_DebugVerbosity::DV_Display);

	/**
	*	Blueprint accessor to SP_LOG_SCREEN.
	*	Use Verbosity color.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Debug")
	static void LogScreen(UObject* Caller, const FString& Str = "Hello", ESP_DebugVerbosity Verbosity = ESP_DebugVerbosity::DV_Display);

	/**
	*	Blueprint accessor to SP_LOG_SCREEN.
	*	Use custom color.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Debug")
	static void LogScreenFull(UObject* Caller, const FString& Str = "Hello", ESP_DebugVerbosity Verbosity = ESP_DebugVerbosity::DV_Display, FLinearColor Color = FLinearColor::Yellow, float Time = 5.0f);
};