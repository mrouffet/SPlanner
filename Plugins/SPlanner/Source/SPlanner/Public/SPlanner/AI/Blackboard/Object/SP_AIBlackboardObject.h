// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include "SP_AIBlackboardObject.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Blackboard")
class SPLANNER_API USP_AIBlackboardObject : public UObject
{
	GENERATED_BODY()

public:
	/** Whether this object is valid. */
	virtual bool IsValidObject() const;

	/**
	*	Reset this object from original.
	*	Must be overridden in children.
	*/
	virtual void Reset(USP_AIBlackboardObject* OriginalObject) /* = 0*/;
};