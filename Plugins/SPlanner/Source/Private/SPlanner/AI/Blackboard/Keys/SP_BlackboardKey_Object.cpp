#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey_Object.h>

UObject* USP_BlackboardKey_Object::GetValue() const
{
	return Handle;
}

void USP_BlackboardKey_Object::SetValue(UObject* Value)
{
	Handle = Value;
}