// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_RequireImplDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

#include <SPlanner/Base/Decorator/SP_PlannerDecoratorFlag.h>

USP_RequireImplDecorator::USP_RequireImplDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SP_SET_FLAG(ValidateMask, ESP_PlannerDecoratorFlag::DF_PreCondition);

#if WITH_EDITOR
	bCanEditValidateMask = false;
#endif
}

bool USP_RequireImplDecorator::PreCondition_Validate_Internal_Implementation(const USP_PlanGenInfos* Infos)
{
	if (bUseRequiredImplOrder)
	{
		// Not enough actions in generated plan.
		if (Infos->OutPlan.Num() < RequieredImpls.Num())
			return false;

		for (int i = 0; i < RequieredImpls.Num(); ++i)
		{
			// PreviousRequieredImpls.Num() and GeneratedPlan.Num() can be different.
			if (RequieredImpls[RequieredImpls.Num() - 1 - i] != Infos->OutPlan[Infos->OutPlan.Num() - 1 - i])
				return false;
		}
	}
	else
	{
		for (int i = 0; i < RequieredImpls.Num(); ++i)
		{
			// Find each required in plan.
			if (Infos->OutPlan.Find(RequieredImpls[i]) == INDEX_NONE)
				return false;
		}
	}

	return Super::PreCondition_Validate_Internal_Implementation(Infos);
}