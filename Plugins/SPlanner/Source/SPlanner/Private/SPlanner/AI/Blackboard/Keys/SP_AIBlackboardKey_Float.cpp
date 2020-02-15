// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Float.h>

#include <SPlanner/Debug/SP_Debug.h>

float USP_AIBlackboardKey_Float::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Float::SetValue(float Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Float::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Float* CastedOther = Cast<USP_AIBlackboardKey_Float>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Float")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Float::CreateInstance()
{
	USP_AIBlackboardKey_Float* NewInstance = NewObject<USP_AIBlackboardKey_Float>(this);
	NewInstance->Handle = Handle;

	return NewInstance;
}