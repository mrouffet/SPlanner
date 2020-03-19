// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorators/Impl/SP_RandDecorator.h>

bool USP_RandDecorator::Validate_Internal_Implementation(const USP_PlannerComponent* Planner)
{
	SP_DECORATOR_SUPER_VALIDATE(Planner)

	return FMath::FRand() <= SuccessRate;
}