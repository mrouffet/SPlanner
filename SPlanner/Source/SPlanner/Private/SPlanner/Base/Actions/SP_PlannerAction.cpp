// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Actions/SP_PlannerAction.h>

#include <SPlanner/Debug/SP_Debug.h>

FSP_PlannerAction FSP_PlannerAction::Make(const FSP_Action* Action, float LODLevel, bool bInAchievePlan, bool bInIsForced)
{
	SP_SRCHECK_NULLPTR(Action, FSP_PlannerAction())

	return FSP_PlannerAction
	{
		Action,
		Action->Weight.Get(LODLevel) * FMath::FRand(), /** Do not Add Action->WeightMultiplier, TimeNumInPlan == 0. */
		bInAchievePlan,
		bInIsForced
	};
}

FSP_PlannerAction FSP_PlannerAction::Make(const FSP_PlannerAction& Other, float LODLevel)
{
	return FSP_PlannerAction
	{
		Other.Handle,
		Other.Handle->Weight.Get(LODLevel) * FMath::FRand() * Other.Handle->WeightMultiplier.GetRichCurveConst()->Eval(Other.TimesNumInPlan + 1),
		Other.bAchievePlan,
		Other.bIsForced,
		Other.TimesNumInPlan + 1
	};
}