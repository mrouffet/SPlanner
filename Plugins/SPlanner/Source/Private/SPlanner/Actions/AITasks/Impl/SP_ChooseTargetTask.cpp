#include <SPlanner/Actions/AITasks/Impl/SP_ChooseTargetTask.h>

#include <SPlanner/Miscs/Flags/SP_AIPlannerFlags.h>

#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>

bool USP_ChooseTargetTask::PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, PlannerFlags)

	const USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);

	// Valid Target component.
	SP_RCHECK_NULLPTR(AIPlanner, false)
	SP_RCHECK_NULLPTR(AIPlanner->Target, false)

	// Not already re-targeted.
	return !SP_IS_FLAG_SET(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
}
uint64 USP_ChooseTargetTask::PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetDirty);
	SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_TargetPosition);

	if(bAutoLookAt)
		SP_ADD_FLAG(PlannerFlags, ESP_AIPlannerFlags::PF_RotationDirty);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetTask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_AI_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	// Valid Target component.
	SP_RCHECK_NULLPTR(Planner->Target, ESP_PlanExecutionState::PES_Failed)

	return ESP_PlanExecutionState::PES_Succeed;
}