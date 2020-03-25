// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_BoolParam.h>

bool USP_BoolParam::Query_Implementation(const UObject* Outer) const
{
	return DefaultValue;
}