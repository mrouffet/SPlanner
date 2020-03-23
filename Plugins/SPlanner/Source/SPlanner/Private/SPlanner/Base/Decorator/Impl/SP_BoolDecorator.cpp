// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_BoolDecorator.h>

#include <SPlanner/Misc/VariableParam/SP_BoolParam.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

bool USP_BoolDecorator::Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	SP_DECORATOR_SUPER_VALIDATE(Planner)

	return Param->Query(Planner);
}