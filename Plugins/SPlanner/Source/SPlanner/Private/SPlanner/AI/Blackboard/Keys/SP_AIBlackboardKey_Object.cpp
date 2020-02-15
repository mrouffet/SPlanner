// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Object.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardObject.h>

USP_AIBlackboardObject* USP_AIBlackboardKey_Object::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Object::SetValue(USP_AIBlackboardObject* Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Object::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Object* CastedOther = Cast<USP_AIBlackboardKey_Object>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Object")

	Handle->Reset(CastedOther->Handle);
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Object::CreateInstance()
{
	USP_AIBlackboardKey_Object* NewInstance = NewObject<USP_AIBlackboardKey_Object>(this);
	NewInstance->Handle = DuplicateObject(Handle, NewInstance);
	
	Handle->Reset(Handle);

	return NewInstance;
}