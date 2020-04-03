// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_IntDecorator.h>

#include <SPlanner/Misc/VariableParam/Int/SP_IntParam.h>

bool USP_IntDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

	return Param->Query(Object) >= Threshold;
}