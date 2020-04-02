// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_BoolParam.h>

bool USP_BoolParam::Query_Implementation(const UObject* Outer) const
{
	return DefaultValue;
}

bool USP_BoolParam::QueryBool_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}
int USP_BoolParam::QueryInt_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}
float USP_BoolParam::QueryFloat_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}