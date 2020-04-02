// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_ScalarParam.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ScalarParam::QueryBool_Implementation(const UObject* Outer) const
{
	SP_LOG(Error, "Method must be overriden in children!")

	return false;
}
int USP_ScalarParam::QueryInt_Implementation(const UObject* Outer) const
{
	SP_LOG(Error, "Method must be overriden in children!")
	
	return 0;
}
float USP_ScalarParam::QueryFloat_Implementation(const UObject* Outer) const
{
	SP_LOG(Error, "Method must be overriden in children!")

	return 0.0f;
}