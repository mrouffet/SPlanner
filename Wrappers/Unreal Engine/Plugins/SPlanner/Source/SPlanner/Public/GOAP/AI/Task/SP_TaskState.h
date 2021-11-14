// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <SP/GOAP/AI/Task/TaskState.hpp>

#include "SP_TaskState.generated.h"

UENUM(BlueprintType, Category = "SPlanner|Task")
enum class ESP_TaskState : uint8
{
	TS_None												UMETA(Hidden),

	TS_Success		= SP::AI::TaskState::Success		UMETA(DisplayName = "Success"),

	TS_Failure		= SP::AI::TaskState::Failure		UMETA(DisplayName = "Failure"),
	TS_BeginFailed	= SP::AI::TaskState::BeginFailed	UMETA(DisplayName = "BeginFailed"),
	TS_EndFailed	= SP::AI::TaskState::EndFailed		UMETA(DisplayName = "EndFailed"),

	TS_Pending		= SP::AI::TaskState::Pending		UMETA(DisplayName = "Pending"),

	TS_ForceEnd		= SP::AI::TaskState::ForceEnd		UMETA(DisplayName = "ForceEnd"),
};
