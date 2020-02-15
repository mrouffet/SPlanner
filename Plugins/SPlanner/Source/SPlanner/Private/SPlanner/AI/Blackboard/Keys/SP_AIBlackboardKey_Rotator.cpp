// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Rotator.h>

const FRotator& USP_AIBlackboardKey_Rotator::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Rotator::SetValue(const FRotator& Value)
{
	Handle = Value;
}