// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_PlannerAction.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>

#include <SPlanner/Base/Action/SP_Action.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

FSP_PlannerAction FSP_PlannerAction::Make(const USP_PlannerComponent* Planner, const USP_Action* Action, bool bInAchievePlan, bool bInIsForced)
{
	SP_SRCHECK_NULLPTR(Action, FSP_PlannerAction())

	return FSP_PlannerAction
	{
		Action,
		Action->Weight->Query(Planner) * FMath::FRand(), /** Do not Add Action->WeightMultiplier, TimeNumInPlan == 0. */
		bInAchievePlan,
		bInIsForced
	};
}

FSP_PlannerAction FSP_PlannerAction::Make(const USP_PlannerComponent* Planner, const FSP_PlannerAction& Other)
{
	return FSP_PlannerAction
	{
		Other.Handle,
		Other.Handle->Weight->Query(Planner) * FMath::FRand() * Other.Handle->WeightMultiplier.GetRichCurveConst()->Eval(Other.TimesNumInPlan + 1),
		Other.bAchievePlan,
		Other.bIsForced,
		Other.TimesNumInPlan + 1
	};
}