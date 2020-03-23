// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Name.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

const FName& USP_AIBlackboardKey_Name::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Name::SetValue(const FName& Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Name::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Name* CastedOther = Cast<USP_AIBlackboardKey_Name>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Name")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Name::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)

	USP_AIBlackboardKey_Name* NewInstance = NewObject<USP_AIBlackboardKey_Name>(Outer);
	NewInstance->Handle = Handle;

	return NewInstance;
}