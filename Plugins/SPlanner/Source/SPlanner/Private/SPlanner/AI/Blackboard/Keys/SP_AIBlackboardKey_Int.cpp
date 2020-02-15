// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Int.h>

#include <SPlanner/Debug/SP_Debug.h>

int USP_AIBlackboardKey_Int::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Int::SetValue(int Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Int::CopyValue(const USP_AIBlackboardKey* Other)
{
	const USP_AIBlackboardKey_Int* CastedOther = Cast<USP_AIBlackboardKey_Int>(Other);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Int")

	Handle = CastedOther->Handle;
}