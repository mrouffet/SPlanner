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

ENUM_CLASS_FLAGS(ESP_DecoratorFlag);