#include <SPlanner/Misc/SP_BlueprintHelper.h>

#include <SPlanner/Debug/SP_Debug.h>

UObject* USP_BlueprintHelper::GetObject(UObject* Object)
{
	/**
	*	Unreal Blueprint editor allow to pickup any loaded object.
	*	This is just an helper getter.
	*/

	SP_SRCHECK_NULLPTR(Object, nullptr)

	return Object;
}