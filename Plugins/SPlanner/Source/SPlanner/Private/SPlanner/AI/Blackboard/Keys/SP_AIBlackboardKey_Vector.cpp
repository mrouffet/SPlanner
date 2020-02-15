
// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey_Vector.h>

#include <SPlanner/Debug/SP_Debug.h>

const FVector& USP_AIBlackboardKey_Vector::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Vector::SetValue(const FVector& Value)
{
	Handle = Value;
}

void USP_AIBlackboardKey_Vector::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Vector* CastedOther = Cast<USP_AIBlackboardKey_Vector>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Vector")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Vector::CreateInstance()
{
	USP_AIBlackboardKey_Vector* NewInstance = NewObject<USP_AIBlackboardKey_Vector>(this);
	NewInstance->Handle = Handle;

	return NewInstance;
}