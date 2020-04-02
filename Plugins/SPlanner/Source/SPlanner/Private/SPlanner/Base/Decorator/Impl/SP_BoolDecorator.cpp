// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_BoolDecorator.h>

#include <SPlanner/Misc/VariableParam/Bool/SP_BoolParam.h>

bool USP_BoolDecorator::Validate_Internal_Implementation(const UObject* Object)
{
	SP_DECORATOR_SUPER_VALIDATE(Object)

	return Param->Query(Object);
}