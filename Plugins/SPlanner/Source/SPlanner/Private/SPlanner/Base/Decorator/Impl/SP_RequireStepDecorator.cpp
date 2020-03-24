// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Decorator/Impl/SP_RequireStepDecorator.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlanGenInfos.h>

#include <SPlanner/Base/Decorator/SP_PlannerDecoratorFlag.h>

USP_RequireStepDecorator::USP_RequireStepDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValidateMask = static_cast<uint8>(ESP_PlannerDecoratorFlag::DF_PreCondition);
}

bool USP_RequireStepDecorator::PreCondition_Validate_Internal_Implementation(const USP_PlanGenInfos* Infos)
{
	if (bUseRequiredStepOrder)
	{
		// Not enough actions in generated plan.
		if (Infos->OutPlan.Num() < RequieredSteps.Num())
			return false;

		for (int i = 0; i < RequieredSteps.Num(); ++i)
		{
			// PreviousRequieredSteps.Num() and GeneratedPlan.Num() can be different.
			if (RequieredSteps[RequieredSteps.Num() - 1 - i] != Infos->OutPlan[Infos->OutPlan.Num() - 1 - i])
				return false;
		}
	}
	else
	{
		for (int i = 0; i < RequieredSteps.Num(); ++i)
		{
			// Find each required in plan.
			if (Infos->OutPlan.Find(RequieredSteps[i]) == INDEX_NONE)
				return false;
		}
	}

	return Super::PreCondition_Validate_Internal_Implementation(Infos);
}

#if WITH_EDITOR
void USP_RequireStepDecorator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Force PreCondition only. 
	if (PropertyChangedEvent.GetPropertyName() == "ValidateMask")
	{
		SP_LOG(Warning, "Decorator must always be checked in PreCondition.")
		ValidateMask = static_cast<uint8>(ESP_PlannerDecoratorFlag::DF_PreCondition);
	}
}
#endif