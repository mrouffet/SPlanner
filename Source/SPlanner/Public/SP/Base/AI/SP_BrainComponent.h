// Copyright 2023 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <BrainComponent.h>
#include "SP_BrainComponent.generated.h"

/**
*	SPlanner base brain component implementation.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|AI")
class SPLANNER_API USP_BrainComponent : public UBrainComponent
{
	GENERATED_BODY()
};
