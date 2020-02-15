// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include "SP_AIBlackboardKey.generated.h"

/**
*	Blackboard key base implementation.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard|AI|Key")
class USP_AIBlackboardKey : public UObject
{
	GENERATED_BODY()
};