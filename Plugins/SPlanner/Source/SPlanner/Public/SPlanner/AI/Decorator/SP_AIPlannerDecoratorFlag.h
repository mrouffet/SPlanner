// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_PlannerDecoratorFlag.h>
#include "SP_AIPlannerDecoratorFlag.generated.h"

/** Debug plan enum flags. */
UENUM(Category = "SPlanner|AI|Decorator", meta = (Bitflags))
enum class ESP_AIPlannerDecoratorFlag : uint8
{
	/** Check decorator to validate Begin. */
	DF_Begin = ESP_PlannerDecoratorFlag::DF_Max		UMETA(DisplayName = "Begin"), // Extend Enum.

	/** Check decorator to validate Tick. */
	DF_Tick											UMETA(DisplayName = "Tick"),

	/** Use this value to extend enum. */
	DF_Max											UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ESP_AIPlannerDecoratorFlag)