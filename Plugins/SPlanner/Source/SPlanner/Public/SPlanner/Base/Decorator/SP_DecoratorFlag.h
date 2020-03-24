// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_DecoratorFlag.generated.h"

/** Debug plan enum flags. */
UENUM(Category = "SPlanner|Decorator", meta = (Bitflags))
enum class ESP_DecoratorFlag : uint8
{
	/** Check decorator to validate availability. */
	DF_Availability				UMETA(DisplayName = "Availability"),

	/** Use this value to extend enum. */
	DF_Max						UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ESP_DecoratorFlag)


// TODO: FIX
/** TEMP FIX EDITOR for Decorator flags. */
UENUM(Category = "SPlanner|Decorator", meta = (Bitflags))
enum class ESP_DecoratorFlag_TEMPFIX : uint8
{
	// --- ESP_DecoratorFlag ---
	/** Check decorator to validate availability. */
	DF_Availability			UMETA(DisplayName = "Availability"),


	// --- ESP_PlannerDecoratorFlag ---
	/** Check decorator to validate Precondition. */
	DF_PreCondition			UMETA(DisplayName = "PreCondition"),


	// --- ESP_AIPlannerDecoratorFlag ---
	/** Check decorator to validate Begin. */
	DF_Begin				UMETA(DisplayName = "Begin"),

	/** Check decorator to validate Tick. */
	DF_Tick					UMETA(DisplayName = "Tick"),

	/** Use this value to extend enum. */
	DF_Max					UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ESP_DecoratorFlag_TEMPFIX)