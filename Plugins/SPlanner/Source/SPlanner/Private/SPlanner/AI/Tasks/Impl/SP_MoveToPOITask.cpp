// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_MoveToPOITask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/AI/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>

bool USP_MoveToPOITask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	// Dirty target will be a POI.
	if (SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPOI))
		return true;

	USP_BlackboardComponent* const Blackboard = Cast<USP_AIPlannerComponent>(&Planner)->GetBlackboard();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)

	// No traget change, check current is POI.
	return Target->GetState() == ESP_TargetState::TS_POI;
}