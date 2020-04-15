// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Bool.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

bool USP_AIBlackboardKey_Bool::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Bool::SetValue(bool Value)
{
	Handle = Value;
}

bool USP_AIBlackboardKey_Bool::Compare(const USP_AIBlackboardKey* OtherKey)
{
	const USP_AIBlackboardKey_Bool* CastedOther = Cast<USP_AIBlackboardKey_Bool>(OtherKey);

	SP_RCHECK(CastedOther, false, "Other is not of type USP_AIBlackboardKey_Bool")

	return Handle == CastedOther->Handle;
}
void USP_AIBlackboardKey_Bool::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Bool* CastedOther = Cast<USP_AIBlackboardKey_Bool>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Bool")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Bool::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)

	USP_AIBlackboardKey_Bool* NewInstance = NewObject<USP_AIBlackboardKey_Bool>(Outer);
	NewInstance->Handle = Handle;

	return NewInstance;
}