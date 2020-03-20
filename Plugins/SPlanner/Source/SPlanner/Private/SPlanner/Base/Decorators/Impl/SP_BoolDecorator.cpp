// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorators/Impl/SP_BoolDecorator.h>

bool USP_BoolDecorator::Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	SP_DECORATOR_SUPER_VALIDATE(Planner)

	return Param.Get();
}