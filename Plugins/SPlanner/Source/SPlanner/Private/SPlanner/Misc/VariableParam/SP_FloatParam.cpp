// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

float USP_FloatParam::Query_Implementation(const UObject* Outer) const
{
	return DefaultValue;
}