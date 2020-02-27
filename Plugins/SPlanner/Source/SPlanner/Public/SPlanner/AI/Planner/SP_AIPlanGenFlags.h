// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include "SP_AIPlanGenFlags.generated.h"

/** flags used during plan generation for AI specialization. */
UENUM(BlueprintType, Category = "SPlanner|Planner|AI")
enum class ESP_AIPlanGenFlags : uint8
{
	/** Pawn location has changed. */
	PG_PawnDirtyLocation												UMETA(DisplayName = "PawnDirtyLocation"),

	/** Pawn location has changed. */
	PG_PawnDirtyRotation												UMETA(DisplayName = "PawnDirtyRotation"),
};

ENUM_CLASS_FLAGS(ESP_AIPlanGenFlags)


/** flags used during plan generation for AI specialization. */
UENUM(BlueprintType, Category = "SPlanner|Planner|AI")
enum class ESP_AIBBPlanGenFlags : uint8
{
	/** Target has changed. */
	PG_Dirty															UMETA(DisplayName = "Dirty"),

	/** The target will a Position. */
	PG_TargetPosition													UMETA(DisplayName = "TargetPosition"),

	/** The target will be an Actor. */
	PG_TargetActor														UMETA(DisplayName = "TargetActor"),

	/** The target will be a Player. */
	PG_TargetPlayer														UMETA(DisplayName = "TargetPlayer"),

	/** The target will be a POI. */
	PG_TargetPOI														UMETA(DisplayName = "TargetPOI"),
};

ENUM_CLASS_FLAGS(ESP_AIBBPlanGenFlags)