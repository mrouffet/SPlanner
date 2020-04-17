// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Int.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

int USP_AIBlackboardKey_Int::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Int::SetValue(int Value)
{
	Handle = Value;
}

bool USP_AIBlackboardKey_Int::Compare(const USP_AIBlackboardKey* OtherKey)
{
	const USP_AIBlackboardKey_Int* CastedOther = Cast<USP_AIBlackboardKey_Int>(OtherKey);

	SP_RCHECK(CastedOther, false, "Other is not of type USP_AIBlackboardKey_Int")

	return Handle == CastedOther->Handle;
}
void USP_AIBlackboardKey_Int::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Int* CastedOther = Cast<USP_AIBlackboardKey_Int>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Int")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Int::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)

	USP_AIBlackboardKey_Int* NewInstance = DuplicateObject(this, Outer);
	NewInstance->Handle = Handle;

	return NewInstance;
}