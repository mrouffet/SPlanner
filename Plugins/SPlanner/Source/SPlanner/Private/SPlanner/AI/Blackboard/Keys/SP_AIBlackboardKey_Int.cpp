// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Int.h>

int USP_AIBlackboardKey_Int::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Int::SetValue(int Value)
{
	Handle = Value;
}