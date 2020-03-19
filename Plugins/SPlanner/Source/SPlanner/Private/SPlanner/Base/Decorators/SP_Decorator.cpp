// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorators/SP_Decorator.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_Decorator::Validate(const USP_PlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(Planner, false)

	bool bInternalResult = Validate_Internal(Planner);

	// Inverse.
	if (bInverseCondition)
		bInternalResult = !bInternalResult;

	// Validation callback.
	if (bInternalResult)
		OnValidationSuccess(Planner);
	else
		OnValidationFailure(Planner);

	return bInternalResult;
}
bool USP_Decorator::Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	return true;
}

bool USP_Decorator::Available_Validate(const USP_PlannerComponent* Planner)
{
	SP_RCHECK_NULLPTR(Planner, false)

	return Available_Validate_Internal(Planner);
}
bool USP_Decorator::Available_Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	return Validate(Planner);
}

bool USP_Decorator::PreCondition_Validate(const USP_PlannerComponent* Planner, const USP_PlanGenInfos* PlanGenInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(PlanGenInfos, false)

	return PreCondition_Validate_Internal(Planner, PlanGenInfos);
}
bool USP_Decorator::PreCondition_Validate_Internal_Implementation(const USP_PlannerComponent* Planner, const USP_PlanGenInfos* PlanGenInfos)
{
	return Validate(Planner);
}

void USP_Decorator::OnValidationSuccess_Implementation(const USP_PlannerComponent* Planner)
{
}
void USP_Decorator::OnValidationFailure_Implementation(const USP_PlannerComponent* Planner)
{
}