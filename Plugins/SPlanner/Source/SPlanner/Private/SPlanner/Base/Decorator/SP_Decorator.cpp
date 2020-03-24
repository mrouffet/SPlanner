// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/SP_Decorator.h>

#include <SPlanner/Debug/SP_Debug.h>

uint8 USP_Decorator::GetValidateMask() const
{
	return ValidateMask;
}

bool USP_Decorator::Validate(const UObject* Object)
{
	SP_RCHECK_NULLPTR(Object, false)

	bool bInternalResult = Validate_Internal(Object);

	// Inverse.
	if (bInverseCondition)
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