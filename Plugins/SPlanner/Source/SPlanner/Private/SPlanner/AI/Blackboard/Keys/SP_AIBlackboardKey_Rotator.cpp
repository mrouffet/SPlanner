// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Rotator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

const FRotator& USP_AIBlackboardKey_Rotator::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Rotator::SetValue(const FRotator& Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Rotator::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Rotator* CastedOther = Cast<USP_AIBlackboardKey_Rotator>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Rotator")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Rotator::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)

	USP_AIBlackboardKey_Rotator* NewInstance = NewObject<USP_AIBlackboardKey_Rotator>(Outer);
	NewInstance->Handle = Handle;

	return NewInstance;
}