
// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey_Vector.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

const FVector& USP_AIBlackboardKey_Vector::GetValue() const
{
	return Handle;
}

void USP_AIBlackboardKey_Vector::SetValue(const FVector& Value)
{
	Handle = Value;
}

bool USP_AIBlackboardKey_Vector::Compare(const USP_AIBlackboardKey* OtherKey)
{
	const USP_AIBlackboardKey_Vector* CastedOther = Cast<USP_AIBlackboardKey_Vector>(OtherKey);

	SP_RCHECK(CastedOther, false, "Other is not of type USP_AIBlackboardKey_Vector")

	return Handle == CastedOther->Handle;
}
void USP_AIBlackboardKey_Vector::ResetValue(const USP_AIBlackboardKey* OriginalKey)
{
	const USP_AIBlackboardKey_Vector* CastedOther = Cast<USP_AIBlackboardKey_Vector>(OriginalKey);

	SP_CHECK(CastedOther, "Other is not of type USP_AIBlackboardKey_Vector")

	Handle = CastedOther->Handle;
}
USP_AIBlackboardKey* USP_AIBlackboardKey_Vector::CreateInstance(USP_AIBlackboardComponent* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)

	USP_AIBlackboardKey_Vector* NewInstance = DuplicateObject(this, Outer);
	NewInstance->Handle = Handle;

	return NewInstance;
}