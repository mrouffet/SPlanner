// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include "SP_BlackboardKey.generated.h"

/**
*	Blackboard key base implementation.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard|Key")
class USP_BlackboardKey : public UObject
{
	GENERATED_BODY()
};