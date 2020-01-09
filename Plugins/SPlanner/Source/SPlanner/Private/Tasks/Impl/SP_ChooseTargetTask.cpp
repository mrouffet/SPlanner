#include <Tasks/Impl/SP_ChooseTargetTask.h>

#include <Components/SP_PlannerComponent.h>

bool USP_ChooseTargetTask::PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_TASK_SUPER_PRECONDITION(Planner, PlannerFlags)

	// Valid Target component.
	SP_RCHECK_NULLPTR(Planner->Target, false)

	// Not already re-targeted.
	return !SP_IS_FLAG_SET(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Dirty);
}
FSP_PlannerFlags USP_ChooseTargetTask::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_TASK_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Dirty);
	SP_ADD_FLAG(PlannerFlags.TargetFlags, ESP_TargetFlags::TF_Position);

	if(bAutoLookAt)
		SP_ADD_FLAG(PlannerFlags.TransformFlags, ESP_TransformFlags::TF_DirtyRotation);

	return PlannerFlags;
}

ESP_PlanExecutionState USP_ChooseTargetTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	// Valid Target component.
	SP_RCHECK_NULLPTR(Planner->Target, ESP_PlanExecutionState::PES_Failed)

	return ESP_PlanExecutionState::PES_Succeed;
}