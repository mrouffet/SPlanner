// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Decorators/SP_AIDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

bool USP_AIDecorator::Begin_Validate_Internal_Implementation(const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos)
{
	return Validate(Planner);
}

bool USP_AIDecorator::Begin_Validate(const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(TaskInfos, false)

	bool bInternalResult = Begin_Validate_Internal(Planner, TaskInfos);

	if (bInverseCondition)
		return !bInternalResult;

	return bInternalResult;
}