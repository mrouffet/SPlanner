// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_ChangeGoalTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

bool USP_ChangeGoalTask::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlanGenInfos)

	const USP_Goal* const PlannerGoal = Planner->GetGoal();

	return PlannerGoal != NewGoal && USP_Goal::CanTransitTo(Planner, PlannerGoal, NewGoal);
}
bool USP_ChangeGoalTask::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	// TODO: Add dirty goal in PlanGenInfos.

	return true;
}
bool USP_ChangeGoalTask::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	// TODO: Add dirty goal in PlanGenInfos.

	return true;
}

bool USP_ChangeGoalTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	SP_RCHECK_NULLPTR(NewGoal, false)

	const USP_Goal* const PlannerGoal = Planner->GetGoal();

	return PlannerGoal != NewGoal && USP_Goal::CanTransitTo(Planner, PlannerGoal, NewGoal);
}
ESP_PlanExecutionState USP_ChangeGoalTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	Planner->SetGoal(NewGoal);

	return ESP_PlanExecutionState::PES_Succeed;
}