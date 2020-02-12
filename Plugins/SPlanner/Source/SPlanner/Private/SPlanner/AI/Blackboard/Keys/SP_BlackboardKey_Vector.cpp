#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Vector.h>

const FVector& USP_BlackboardKey_Vector::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Vector::SetValue(const FVector& Value)
{
	Handle = Value;
}