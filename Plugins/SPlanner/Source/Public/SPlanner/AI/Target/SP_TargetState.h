#pragma once

#include  "SP_TargetState.generated.h"

/**
*	Enum state of TargetComponent.
*/
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Target")
enum class ESP_TargetState : uint8
{
	TS_None				UMETA(DisplayName = "None"),

	TS_Position			UMETA(DisplayName = "Position"),
	TS_Actor			UMETA(DisplayName = "Actor"),
	TS_Player			UMETA(DisplayName = "Player"),
	TS_POI				UMETA(DisplayName = "POI"),
};