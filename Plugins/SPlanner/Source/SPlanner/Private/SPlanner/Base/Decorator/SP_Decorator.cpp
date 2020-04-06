// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/SP_Decorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/Base/Decorator/SP_DecoratorFlag.h>

uint8 USP_Decorator::GetValidateMask() const
{
	return ValidateMask;
}

USP_Decorator::USP_Decorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SP_SET_FLAG(ValidateMask, ESP_DecoratorFlag::DF_Availability);
}

bool USP_Decorator::Validate(const UObject* Object)
{
	SP_RCHECK_NULLPTR(Object, false)

	bool bInternalResult = Validate_Internal(Object);

	// Inverse.
	if (bReversedCondition)
		bInternalResult = !bInternalResult;

	// Validation callback.
	if (bInternalResult)
		OnValidationSuccess(Object);
	else
		OnValidationFailure(Object);

	return bInternalResult;
}
bool USP_Decorator::Validate_Internal_Implementation(const UObject* Object)
{
	return true;
}

bool USP_Decorator::Available_Validate(const UObject* Object)
{
	SP_RCHECK_NULLPTR(Object, false)

	return Available_Validate_Internal(Object);
}
bool USP_Decorator::Available_Validate_Internal_Implementation(const UObject* Object)
{
	return Validate(Object);
}

void USP_Decorator::OnValidationSuccess_Implementation(const UObject* Object)
{
}
void USP_Decorator::OnValidationFailure_Implementation(const UObject* Object)
{
}

void USP_Decorator::PostExecution(const UObject* Object, bool bExecutionSuccess)
{
	PostExecution_Internal(Object, bExecutionSuccess);
}
void USP_Decorator::PostExecution_Internal_Implementation(const UObject* Object, bool bExecutionSuccess)
{
	SP_CHECK_NULLPTR(Object)
}


#if WITH_EDITOR
bool USP_Decorator::CanEditChange(const UProperty* InProperty) const
{
	if (!Super::CanEditChange(InProperty))
		return false;

	// Mask no more editable (from child implementation).
	if (!bCanEditValidateMask && InProperty->GetName() == "ValidateMask")
		return false;

	return true;
}
#endif