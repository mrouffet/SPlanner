// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_PlannerDecorator.generated.h"

class USP_PlanGenInfos;
class USP_PlannerComponent;

/**
 *	SPlanner decorator for planner base implementation.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_PlannerDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** PreCondition validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool PreCondition_Validate_Internal(const USP_PlanGenInfos* Infos);

public:
	/**
	*	PreCondition validate implementation.
	*	Called by SP_TaskStep while this decorator is in PreConditionDecorators list.
	*	Call PreCondition_Validate_Internal.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool PreCondition_Validate(const USP_PlanGenInfos* Infos);
};