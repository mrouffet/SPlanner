#include <SPlanner/Base/Actions/SP_ActionBase.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Actions/SP_ActionStep.h>

FSP_ActionBase::FSP_ActionBase(USP_ActionStep* InStep, ESP_ActionGenerationType InGenerationType) :
	Step{ InStep },
	GenerationType{ InGenerationType }
{
}

#if WITH_EDITOR

FString FSP_ActionBase::GetDebugStr() const
{
	SP_SRCHECK_NULLPTR(Step, FString("None"))

	return Step->GetName();
}

#endif