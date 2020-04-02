// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/ChooseTarget/SP_ChooseTargetPOITask.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Blackboard/Object/Target/SP_Target.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

void USP_ChooseTargetPOITask::SetPOITarget(USP_AIPlannerComponent* Planner, USP_POIComponent* POI)
{
	SP_CHECK_NULLPTR(POI)
	SP_CHECK_NULLPTR(Planner)

	const USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_CHECK_NULLPTR(Blackboard)

	USP_Target* const OutTarget = Blackboard->GetObject<USP_Target>(OutTargetEntryName);
	SP_CHECK_NULLPTR(OutTarget)

	OutTarget->SetPOI(POI);
}

bool USP_ChooseTargetPOITask::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->AddBlackboardFlag(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetPOI);

	return true;
}
bool USP_ChooseTargetPOITask::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	USP_AIPlanGenInfos* const AIPlanGenInfos = Cast<USP_AIPlanGenInfos>(Infos);
	SP_RCHECK_NULLPTR(AIPlanGenInfos, false)

	AIPlanGenInfos->RemoveBlackboardFlag(OutTargetEntryName, ESP_AIBBPlanGenFlags::PG_TargetPOI);

	return true;
}