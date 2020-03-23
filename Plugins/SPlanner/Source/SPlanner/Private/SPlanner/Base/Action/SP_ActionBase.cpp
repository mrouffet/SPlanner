// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionBase.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Action/SP_ActionStep.h>

//#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

//FSP_ActionBase::FSP_ActionBase(USP_ActionStep* InStep, ESP_ActionGenerationType InGenerationType) :
//	Step{ InStep },
//	GenerationType{ InGenerationType }
//{
//}

bool USP_ActionBase::CheckAvailability(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Step, false)
	SP_RCHECK_NULLPTR(Planner, false)

	return Step->CheckAvailability(Planner);
}

#if WITH_EDITOR

FString USP_ActionBase::GetDebugStr() const
{
	SP_RCHECK_NULLPTR(Step, FString("None"))

	return Step->GetName();
}

#endif