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

void USP_AIBlackboardKey_Bool::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Bool* CastedOther = Cast<USP_AIBlackboardKey_Bool>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Bool")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Bool::CreateInstance()
{
	USP_AIBlackboardKey_Bool* NewInstance = NewObject<USP_AIBlackboardKey_Bool>(this);
	NewInstance->Handle = Handle;

	return NewInstance;
}