// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionStep.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <SPlanner/Base/Decorator/SP_PlannerDecorator.h>
#include <SPlanner/Base/Decorator/SP_PlannerDecoratorFlag.h>

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

		if (SP_IS_FLAG_SET(Decorators[i]->GetValidateMask(), ESP_PlannerDecoratorFlag::DF_PreCondition))
		{
			// Try cast as USP_PlannerDecorator.
			if (USP_PlannerDecorator* const PlannerDecorator = Cast<USP_PlannerDecorator>(Decorators[i]))
			{
				if(!PlannerDecorator->PreCondition_Validate(Infos))
					return false;
			}
			else if(Decorators[i]->Validate(Infos->Planner)) // Try base validate using Planner.
				return false;
		}
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