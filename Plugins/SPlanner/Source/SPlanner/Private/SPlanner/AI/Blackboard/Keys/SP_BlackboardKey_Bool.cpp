// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Bool.h>

bool USP_BlackboardKey_Bool::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Bool::SetValue(bool Value)
{
	Handle = Value;
}