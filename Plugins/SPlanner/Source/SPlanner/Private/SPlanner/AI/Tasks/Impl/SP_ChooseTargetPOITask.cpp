// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetPOITask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

void USP_ChooseTargetPOITask::SetPOITarget(USP_AIPlannerComponent* Planner, USP_POIComponent* POI)
{
	SP_CHECK_NULLPTR(POI)
	SP_CHECK_NULLPTR(Planner)

	USP_AIBlackboardComponent* const Blackboard = Planner->GetBlackboard<USP_AIBlackboardComponent>();
	SP_CHECK_NULLPTR(Blackboard)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_CHECK_NULLPTR(Target)

	Target->SetPOI(POI);
}

uint64 USP_ChooseTargetPOITask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPOI);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetActor);

	return PlannerFlags;
}