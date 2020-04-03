// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableAsset/SP_ScalarAsset.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_ScalarAsset::GetBool() const
{
	SP_LOG(Error, "Method must be overriden in children!")

	return false;
}
void USP_ScalarAsset::SetBool(bool InValue)
{
	SP_LOG(Error, "Method must be overriden in children!")
}

int USP_ScalarAsset::GetInt() const
{
	SP_LOG(Error, "Method must be overriden in children!")

	return 0;
}
void USP_ScalarAsset::SetInt(int InValue)
{
	SP_LOG(Error, "Method must be overriden in children!")
}

float USP_ScalarAsset::GetFloat() const
{
	SP_LOG(Error, "Method must be overriden in children!")

	return 0.0f;
}
void USP_ScalarAsset::SetFloat(float InValue)
{
	SP_LOG(Error, "Method must be overriden in children!")
}