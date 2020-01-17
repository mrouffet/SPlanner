#include <SPlanner/Actions/SP_ActionBase.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Actions/SP_ActionStep.h>

FSP_ActionBase::FSP_ActionBase(USP_ActionStep* InStep) : Step{ InStep }
{
}

#if WITH_EDITOR
FString FSP_ActionBase::GetDebugStr() const
{
	SP_SRCHECK_NULLPTR(Step, FString())

	return Step->GetName();
}
#endif