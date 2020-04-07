// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Decorator/SP_DecoratorMacro.h>

#include <SPlanner/Framework/SP_Object.h>
#include "SP_Decorator.generated.h"

/**
 *	SPlanner decorator base implementation.
 *	Decorator are additionnal conditions, which must be validated for a SP_TaskImpl to be executed.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_Decorator : public USP_Object
{
	GENERATED_BODY()
	
protected:
	/** Whether the condition should be reversed (ie NOT condition). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool bReversedCondition = false;

	/** Mask to call validate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ESP_DecoratorFlag_TEMPFIX")) // TODO: FIX.
	uint8 ValidateMask = 0u; // init in .cpp

	/** Common validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Validate_Internal(const UObject* Object);

	/** Available validate implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	bool Available_Validate_Internal(const UObject* Object);

	/** Callback method called when this get validated. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void OnValidationSuccess(const UObject* Object);

	/** Callback method called when this get validated. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void OnValidationFailure(const UObject* Object);

	/** PostExecution implementation. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Decorator")
	void PostExecution_Internal(const UObject* Object, bool bExecutionSuccess);

#if WITH_EDITOR
	/** Whether ValidateMask can be edited from Editor window. */
	bool bCanEditValidateMask = true;
#endif

public:
	USP_Decorator(const FObjectInitializer& ObjectInitializer);

	/** Getter of ValidateMask. */
	uint8 GetValidateMask() const;

	/**
	*	Default validate implementation.
	*	Call Validate_Internal().
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool Validate(const UObject* Object);

	/**
	*	Available validate implementation.
	*	Called by SP_PlannerActionSet while this decorator is in AvailabilityDecorators list.
	*	Call Available_Validate_Internal.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	bool Available_Validate(const UObject* Object);

	/**
	*	Method to call after the execution of the decorator's object has ended.
	*	Call PostExecution_Internal().
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Decorator")
	void PostExecution(const UObject* Object, bool bExecutionSuccess = true);

#if WITH_EDITOR
	bool CanEditChange(const UProperty* InProperty) const override;
#endif
};