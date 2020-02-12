#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Int.h>

int USP_BlackboardKey_Int::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Int::SetValue(int Value)
{
	Handle = Value;
}