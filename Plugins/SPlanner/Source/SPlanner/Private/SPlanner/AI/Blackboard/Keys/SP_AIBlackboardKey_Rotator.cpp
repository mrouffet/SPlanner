// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Rotator.h>

#include <SPlanner/Debug/SP_Debug.h>

const FRotator& USP_AIBlackboardKey_Rotator::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Rotator::SetValue(const FRotator& Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Rotator::CopyValue(const USP_AIBlackboardKey* Other)
{
	const USP_AIBlackboardKey_Rotator* CastedOther = Cast<USP_AIBlackboardKey_Rotator>(Other);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Rotator")

	Handle = CastedOther->Handle;
}