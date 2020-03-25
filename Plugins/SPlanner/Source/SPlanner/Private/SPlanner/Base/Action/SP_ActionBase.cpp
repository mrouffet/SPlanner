// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionBase.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Action/SP_ActionStep.h>

bool USP_ActionBase::IsAvailable(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Step, false)
	SP_RCHECK_NULLPTR(Planner, false)

	return Step->IsAvailable(Planner);
}

#if WITH_EDITOR

FString USP_ActionBase::GetDebugStr() const
{
	SP_RCHECK_NULLPTR(Step, FString("None"))

	return Step->GetName();
}

#endif