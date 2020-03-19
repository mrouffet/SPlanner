// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_ActionBase.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Actions/SP_ActionStep.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

FSP_ActionBase::FSP_ActionBase(USP_ActionStep* InStep, ESP_ActionGenerationType InGenerationType) :
	Step{ InStep },
	GenerationType{ InGenerationType }
{
}

bool FSP_ActionBase::CheckAvailability(const USP_PlannerComponent* Planner) const
{
	SP_SRCHECK_NULLPTR(Step, false)
	SP_SRCHECK_NULLPTR(Planner, false)

	return Step->CheckAvailability(Planner);
}

#if WITH_EDITOR

FString FSP_ActionBase::GetDebugStr() const
{
	SP_SRCHECK_NULLPTR(Step, FString("None"))

	return Step->GetName();
}

#endif