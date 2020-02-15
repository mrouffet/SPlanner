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

public:
	/**
	*	reset the value from the original key.
	*	Must be overridden in children.
	*/
	virtual void ResetValue(const USP_AIBlackboardKey* OriginalKey) /* = 0 */;

	/**
	*	Create a new instance of this key.
	*	Must be overridden in children.
	*/
	virtual USP_AIBlackboardKey* CreateInstance() /* = 0 */;
};