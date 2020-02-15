// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Bool.h>

bool USP_AIBlackboardKey_Bool::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Bool::SetValue(bool Value)
{
	Handle = Value;
}