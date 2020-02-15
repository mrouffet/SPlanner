// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Name.h>

#include <SPlanner/Debug/SP_Debug.h>

const FName& USP_AIBlackboardKey_Name::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Name::SetValue(const FName& Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Name::CopyValue(const USP_AIBlackboardKey* Other)
{
	const USP_AIBlackboardKey_Name* CastedOther = Cast<USP_AIBlackboardKey_Name>(Other);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Name")

	Handle = CastedOther->Handle;
}