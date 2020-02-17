// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_NullTask.generated.h"

/**
 *	Null task implementation.
 *	This task has no implementation.
 *	Use it to force end a plan.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_NullTask : public USP_Task
{
	GENERATED_BODY()
};