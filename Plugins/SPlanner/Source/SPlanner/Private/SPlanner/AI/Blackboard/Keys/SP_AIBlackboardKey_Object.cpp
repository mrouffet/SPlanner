// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Object.h>

UObject* USP_AIBlackboardKey_Object::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Object::SetValue(UObject* Value)
{
	Handle = Value;
}