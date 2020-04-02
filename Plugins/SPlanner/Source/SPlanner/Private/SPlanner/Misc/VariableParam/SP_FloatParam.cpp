// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

float USP_FloatParam::Query_Implementation(const UObject* Outer) const
{
	return DefaultValue;
}

bool USP_FloatParam::QueryBool_Implementation(const UObject* Outer) const
{
	return Query(Outer) != 0.0f;
}
int USP_FloatParam::QueryInt_Implementation(const UObject* Outer) const
{
	return static_cast<int>(Query(Outer));
}
float USP_FloatParam::QueryFloat_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}