// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Object.h>

#include <SPlanner/Debug/SP_Debug.h>

UObject* USP_AIBlackboardKey_Object::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Object::SetValue(UObject* Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Object::CopyValue(const USP_AIBlackboardKey* Other)
{
	const USP_AIBlackboardKey_Object* CastedOther = Cast<USP_AIBlackboardKey_Object>(Other);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Object")

	Handle = CastedOther->Handle;
}