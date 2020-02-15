// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Bool.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_AIBlackboardKey_Bool::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Bool::SetValue(bool Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Bool::CopyValue(const USP_AIBlackboardKey* Other)
{
	const USP_AIBlackboardKey_Bool* CastedOther = Cast<USP_AIBlackboardKey_Bool>(Other);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Bool")

	Handle = CastedOther->Handle;
}