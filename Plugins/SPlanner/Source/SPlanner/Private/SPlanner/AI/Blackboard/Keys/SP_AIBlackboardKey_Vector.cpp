// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Vector.h>

const FVector& USP_AIBlackboardKey_Vector::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Vector::SetValue(const FVector& Value)
{
	Handle = Value;
}