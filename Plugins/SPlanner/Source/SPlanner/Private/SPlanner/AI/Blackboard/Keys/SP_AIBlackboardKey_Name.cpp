// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Name.h>

const FName& USP_AIBlackboardKey_Name::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Name::SetValue(const FName& Value)
{
	Handle = Value;
}