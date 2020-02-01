// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorators/SP_DecoratorMacro.h>

#include <UObject/Object.h>
#include "SP_Decorator.generated.h"

class USP_PlanGenInfos;
class USP_PlannerComponent;

/**
 *	SPlanner decorator base implementation.
 *	Decorator are additionnal conditions, which must be validated for a SP_Task to be executed.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_Decorator : public UObject
{
	GENERATED_BODY()
	
protected:
	/** Whether the condition should be inverted (ie NOT condition). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool bInverseCondition = false;

	/** Common validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Validate_Internal(const USP_PlannerComponent* Planner);

	/** Available validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Available_Validate_Internal(const USP_PlannerComponent* Planner);

	/** PreCondition validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool PreCondition_Validate_Internal(const USP_PlannerComponent* Planner, const USP_PlanGenInfos* PlanGenInfos);

	/** Callback method called when this get validated. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void OnValidationSuccess(const USP_PlannerComponent* Planner);

	/** Callback method called when this get validated. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void OnValidationFailure(const USP_PlannerComponent* Planner);

public:
	/**
	*	Default validate implementation.
	*	Call Validate_Internal().
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool Validate(const USP_PlannerComponent* Planner);

	/**
	*	Available validate implementation.
	*	Called by SP_PlannerActionSet while this decorator is in AvailabilityDecorators list.
	*	Call Available_Validate_Internal.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool Available_Validate(const USP_PlannerComponent* Planner);

	/**
	*	PreCondition validate implementation.
	*	Called by SP_Task while this decorator is in PreConditionDecorators list.
	*	Call PreCondition_Validate_Internal.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool PreCondition_Validate(const USP_PlannerComponent* Planner, const USP_PlanGenInfos* PlanGenInfos);
};