#include <Tasks/Impl/ChooseTargetTask.h>

#include <Miscs/PlannerFlags.h>
#include <Components/PlannerComponent.h>

bool USP_ChooseTargetTask::PreCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_PRECONDITION(Planner, PlannerFlags)

	// Valid target component and not already re-targeted.
	return Planner->GetTarget() && !IS_FLAG_SET(PlannerFlags, ESP_PlannerFlags::PF_DirtyTarget);
}
int USP_ChooseTargetTask::PostCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_POSTCONDITION(Planner, PlannerFlags)

	return ADD_FLAG(PlannerFlags, ESP_PlannerFlags::PF_DirtyTarget);
}

ESP_PlanExecutionState USP_ChooseTargetTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	SP_RCHECK_NULLPTR(Planner->GetTarget(), ESP_PlanExecutionState::PES_Failed)

	return ESP_PlanExecutionState::PES_Succeed;
}