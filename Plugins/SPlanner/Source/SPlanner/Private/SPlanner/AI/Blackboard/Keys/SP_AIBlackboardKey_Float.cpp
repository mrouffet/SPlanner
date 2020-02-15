// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Float.h>

float USP_AIBlackboardKey_Float::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Float::SetValue(float Value)
{
	Handle = Value;
}