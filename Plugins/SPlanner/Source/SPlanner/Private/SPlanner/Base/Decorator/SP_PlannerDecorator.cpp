// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/SP_PlannerDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

bool USP_PlannerDecorator::PreCondition_Validate(const USP_PlanGenInfos* Infos)
{
	SP_RCHECK_NULLPTR(Infos, false)

	return PreCondition_Validate_Internal(Infos);
}
bool USP_PlannerDecorator::PreCondition_Validate_Internal_Implementation(const USP_PlanGenInfos* Infos)
{
	// Forward Planner
	return Validate(Infos->Planner);
}