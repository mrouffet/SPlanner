// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_TaskNotification.generated.h"

/**
*	How task should react to notification.
*/
UENUM(BlueprintType, Category = "SPlanner|Task")
enum class ESP_TaskNotification : uint8
{
	/** Start tick, notify cause timeout. */
	TNA_TimeOut				UMETA(DisplayName = "TimeOut"),

	/** Wait for notify before tick. */
	TNA_WaitForNotify		UMETA(DisplayName = "WaitForNotify"),
};