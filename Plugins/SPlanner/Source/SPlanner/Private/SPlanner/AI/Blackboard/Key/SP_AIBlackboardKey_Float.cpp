// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Float.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

float USP_AIBlackboardKey_Float::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Float::SetValue(float Value)
{
	Handle = Value;
}

bool USP_AIBlackboardKey_Float::Compare(const USP_AIBlackboardKey* OtherKey)
{
	const USP_AIBlackboardKey_Float* CastedOther = Cast<USP_AIBlackboardKey_Float>(OtherKey);

	SP_RCHECK(CastedOther, false, "Other is not of type USP_AIBlackboardKey_Float")

	return FMath::IsNearlyEqual(Handle, CastedOther->Handle);
}
void USP_AIBlackboardKey_Float::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Float* CastedOther = Cast<USP_AIBlackboardKey_Float>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Float")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Float::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)

	USP_AIBlackboardKey_Float* NewInstance = NewObject<USP_AIBlackboardKey_Float>(Outer);
	NewInstance->Handle = Handle;

	return NewInstance;
}