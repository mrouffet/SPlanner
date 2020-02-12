// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Float.h>

float USP_BlackboardKey_Float::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Float::SetValue(float Value)
{
	Handle = Value;
}