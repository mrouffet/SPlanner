// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_DecoratorFlag.h>
#include <SPlanner/Base/Decorator/SP_DecoratorMacro.h>

#include <SPlanner/Framework/SP_Object.h>
#include "SP_Decorator.generated.h"

class USP_PlanGenInfos;
class USP_PlannerComponent;

/**
 *	SPlanner decorator base implementation.
 *	Decorator are additionnal conditions, which must be validated for a SP_TaskStep to be executed.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_Decorator : public USP_Object
{
	GENERATED_BODY()
	
protected:
	/** Whether the condition should be inverted (ie NOT condition). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool bInverseCondition = false;

	/** Mask to call validate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ESP_DecoratorFlag"))
	uint8 ValidateMask = static_cast<uint8>(ESP_DecoratorFlag::DF_Availability);

	/** Common validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Validate_Internal(const USP_PlannerComponent* Planner);

	/** Available validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Available_Validate_Internal(const USP_PlannerComponent* Planner);

	/** PreCondition validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool PreCondition_Validate_Internal(const USP_PlanGenInfos* Infos);

	/** Callback method called when this get validated. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void OnValidationSuccess(const USP_PlannerComponent* Planner);

	/** Callback method called when this get validated. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void OnValidationFailure(const USP_PlannerComponent* Planner);

public:
	/** Getter of ValidateMask. */
	uint8 GetValidateMask() const;

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
	*	Called by SP_TaskStep while this decorator is in PreConditionDecorators list.
	*	Call PreCondition_Validate_Internal.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool PreCondition_Validate(const USP_PlanGenInfos* Infos);
};