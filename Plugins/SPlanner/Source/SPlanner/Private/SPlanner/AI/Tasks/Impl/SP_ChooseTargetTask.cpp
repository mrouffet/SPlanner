// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChooseTargetTask.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <SPlanner/AI/Target/SP_Target.h>
#include <SPlanner/AI/Planner/SP_AIPlannerFlags.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

bool USP_ChooseTargetTask::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

#if SP_DEBUG
	// Check valid blackboard entry.
	USP_AIBlackboardComponent* const Blackboard = Planner.GetBlackboard<USP_AIBlackboardComponent>();
	SP_RCHECK_NULLPTR(Blackboard, false)

	USP_Target* const Target = Blackboard->GetObject<USP_Target>(TargetEntryName);
	SP_RCHECK_NULLPTR(Target, false)
#endif

	// Not already re-targeted.
	if(!bAllowReTarget && SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty))
		return false;

	return true;
}
uint64 USP_ChooseTargetTask::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPosition);

	return PlannerFlags;
}