// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_FormationConstructionType.generated.h"

/** enum of formation construct type. */
UENUM(Category = "SPlanner|AI|Formation")
enum class ESP_FormationConstructionType : uint8
{
	/** Construct by dichotomy. */
	FCT_Dichotomy			UMETA(DisplayName = "Dichotomy"),


	/** Construct point by point in order. */
	FCT_PointByPoint		UMETA(DisplayName = "PointByPoint"),
};