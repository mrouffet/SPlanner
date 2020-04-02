// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_IntParam.h>

int USP_IntParam::Query_Implementation(const UObject* Outer) const
{
	return DefaultValue;
}

bool USP_IntParam::QueryBool_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}
int USP_IntParam::QueryInt_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}
float USP_IntParam::QueryFloat_Implementation(const UObject* Outer) const
{
	return Query(Outer);
}