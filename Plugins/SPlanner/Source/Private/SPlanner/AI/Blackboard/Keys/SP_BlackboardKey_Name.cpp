#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Name.h>

const FName& USP_BlackboardKey_Name::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Name::SetValue(const FName& Value)
{
	Handle = Value;
}