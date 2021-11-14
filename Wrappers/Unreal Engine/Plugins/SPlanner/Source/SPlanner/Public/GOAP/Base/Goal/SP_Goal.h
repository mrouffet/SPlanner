// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Abstract/SP_DataAsset.h>

#include <SP/GOAP/Base/Goal/AGoal.hpp>

#include "SP_Goal.generated.h"

/**
 *
 */
UCLASS(Blueprintable, ClassGroup = "SPlanner|Goal")
class SPLANNER_API USP_Goal : public USP_DataAsset, public SP::AGoal
{
	GENERATED_BODY()

protected:

};
