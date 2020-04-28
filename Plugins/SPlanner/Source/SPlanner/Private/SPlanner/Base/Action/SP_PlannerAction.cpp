// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_PlannerAction.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/VariableParam/Float/SP_FloatCurveParam.h>

#include <SPlanner/Base/Action/SP_Action.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

FSP_PlannerAction FSP_PlannerAction::Make(const USP_PlannerComponent* Planner, const USP_Action* Action, bool bInAchievePlan, bool bInIsForced)
{
	SP_SRCHECK_NULLPTR(Action, FSP_PlannerAction())

	USP_FloatCurveParamInfos* WeightInfos = nullptr;

	// Should create float curve infos. TODO: CLEAN.
	if (Action->GenerationType == ESP_ActionGenerationType::AGT_RandomWeight && Cast<USP_FloatCurveParam>(Action->Weight))
	{
		WeightInfos = NewObject<USP_FloatCurveParamInfos>();

		WeightInfos->CurveTime = 0;
		WeightInfos->Outer = Planner;
	}

	return FSP_PlannerAction
	{
		Action,
		(WeightInfos ? Action->Weight->Query(WeightInfos) : Action->Weight->Query(Planner))  * FMath::FRand(),
		bInAchievePlan,
		bInIsForced
	};
}

FSP_PlannerAction FSP_PlannerAction::Make(const USP_PlannerComponent* Planner, const FSP_PlannerAction& Other)
{
	SP_SRCHECK_NULLPTR(Other.Handle, FSP_PlannerAction())

	USP_FloatCurveParamInfos* WeightInfos = nullptr;

	// Should create float curve infos. TODO: CLEAN.
	if (Other.Handle->GenerationType == ESP_ActionGenerationType::AGT_RandomWeight && Cast<USP_FloatCurveParam>(Other.Handle->Weight))
	{
		WeightInfos = NewObject<USP_FloatCurveParamInfos>();

		WeightInfos->Outer = Planner;
		WeightInfos->CurveTime = Other.TimesNumInPlan + 1;
	}

	return FSP_PlannerAction
	{
		Other.Handle,
		(WeightInfos ? Other.Handle->Weight->Query(WeightInfos) : Other.Handle->Weight->Query(Planner))  * FMath::FRand(),
		Other.bAchievePlan,
		Other.bIsForced,
		Other.TimesNumInPlan + 1
	};
}