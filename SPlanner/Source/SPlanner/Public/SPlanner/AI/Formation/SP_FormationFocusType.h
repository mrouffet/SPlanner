// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_FormationFocusType.generated.h"

/** enum of formation focus action. */
UENUM(Category = "SPlanner|AI|Formation")
enum class ESP_FormationFocusType : uint8
{
	/** Don't perform focus. */
	FFT_None			UMETA(DisplayName = "None"),

	/** Focus Lead actor. */
	FFT_Lead			UMETA(DisplayName = "Lead"),

	/** Focus Target actor. */
	FFT_Target			UMETA(DisplayName = "Target"),

	/** Clear the previous focused actor. */
	FFT_Clear			UMETA(DisplayName = "Clear"),
};