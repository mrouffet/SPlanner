#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Rotator.h>

const FRotator& USP_BlackboardKey_Rotator::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Rotator::SetValue(const FRotator& Value)
{
	Handle = Value;
}