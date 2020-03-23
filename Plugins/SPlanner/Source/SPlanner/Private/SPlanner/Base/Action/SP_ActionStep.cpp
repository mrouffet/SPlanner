// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

#include <SPlanner/Base/Decorator/SP_Decorator.h>

bool USP_ActionStep::CheckAvailability(const USP_PlannerComponent* Planner) const
{
	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] nullptr!", i)

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_DecoratorFlag::DF_Availability) &&
			!Decorators[i]->Available_Validate(Planner))
			return false;
	}

	return true;
}

bool USP_ActionStep::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_RCHECK_NULLPTR(Infos, false)
	SP_RCHECK_NULLPTR(Infos->Planner, false)

	// Check decorators.
	for (int i = 0; i < Decorators.Num(); ++i)
	{
		SP_CCHECK(Decorators[i], "Decorators[%d] nullptr!", i)

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_DecoratorFlag::DF_PreCondition) &&
			!Decorators[i]->PreCondition_Validate(Infos))
			return false;
	}

	return true;
}
bool USP_ActionStep::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_RCHECK_NULLPTR(Infos, false)
	SP_RCHECK_NULLPTR(Infos->Planner, false)

	return true;
}

bool USP_ActionStep::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_RCHECK_NULLPTR(Infos, false)
	SP_RCHECK_NULLPTR(Infos->Planner, false)

	return true;
}