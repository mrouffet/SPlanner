// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_Decorator.h>
#include "SP_AIDecorator.generated.h"

class USP_TaskInfos;
class USP_AIPlannerComponent;

/**
 *	SPlanner decorator AI implementation.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_AIDecorator : public USP_Decorator
{
	GENERATED_BODY()
	
protected:
	/** Begin validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Begin_Validate_Internal(const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos);

	/** Begin validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Tick_Validate_Internal(float DeltaSeconds, const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos);

public:
	/**
	*	Begin validate implementation.
	*	Called by SP_TaskStep while this decorator is in BeginDecorators list.
	*	Call Begin_Validate_Internal.
	*/
	bool Begin_Validate(const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos);

	/**
	*	Tick validate implementation.
	*	Called by SP_TaskStep while this decorator is in TickDecorators list.
	*	Call Tick_Validate_Internal.
	*/
	bool Tick_Validate(float DeltaSeconds, const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos);
};