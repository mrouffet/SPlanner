// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_DecoratorFlag.h>
#include "SP_PlannerDecoratorFlag.generated.h"

/** Debug plan enum flags. */
UENUM(Category = "SPlanner|Decorator", meta = (Bitflags))
enum class ESP_PlannerDecoratorFlag : uint8
{
	/** Check decorator to validate Precondition. */
	DF_PreCondition = ESP_DecoratorFlag::DF_Max		UMETA(DisplayName = "PreCondition"), // Extend Enum.

	/** Use this value to extend enum. */
	DF_Max											UMETA(Hidden)
};

ENUM_CLASS_FLAGS(ESP_PlannerDecoratorFlag)