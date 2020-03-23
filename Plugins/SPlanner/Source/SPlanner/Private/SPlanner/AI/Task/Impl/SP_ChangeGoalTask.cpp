// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/SP_ChangeGoalTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

bool USP_ChangeGoalTask::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	const USP_Goal* const PlannerGoal = Infos->Planner->GetGoal();

	return PlannerGoal != NewGoal && USP_Goal::CanTransitTo(Infos->Planner, PlannerGoal, NewGoal);
}
bool USP_ChangeGoalTask::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	// TODO: Add dirty goal in Infos.

	return true;
}
bool USP_ChangeGoalTask::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	// TODO: Add dirty goal in Infos.

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