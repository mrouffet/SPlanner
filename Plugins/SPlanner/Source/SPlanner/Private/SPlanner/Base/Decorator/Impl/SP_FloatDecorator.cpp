// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_FloatDecorator.h>

#include <SPlanner/Misc/VariableParam/Float/SP_FloatParam.h>

bool USP_FloatDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

	return Param->Query(Object) >= Threshold;
}