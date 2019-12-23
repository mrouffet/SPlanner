#include <Tasks/Target/TargetTask.h>

#include <Debug/Debug.h>

#include <Components/PlannerComponent.h>

bool USP_TargetTask::PreCondition_Internal(const USP_PlannerComponent* Planner, const USP_TargetComponent* Target, int PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Target, false)

	return true;
}
int USP_TargetTask::PostCondition_Internal(const USP_PlannerComponent* Planner, const USP_TargetComponent* Target, int PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Target, PlannerFlags)

	return PlannerFlags;
}

ESP_PlanExecutionState USP_TargetTask::Begin_Internal(USP_PlannerComponent* Planner, USP_TargetComponent* Target, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

		return ESP_PlanExecutionState::PES_Succeed;
}
ESP_PlanExecutionState USP_TargetTask::Tick_Internal(float DeltaSeconds, USP_PlannerComponent* Planner, USP_TargetComponent* Target, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	return ESP_PlanExecutionState::PES_Succeed;
}
ESP_PlanExecutionState USP_TargetTask::End_Internal(USP_PlannerComponent* Planner, USP_TargetComponent* Target, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(Target, ESP_PlanExecutionState::PES_Failed)

	return ESP_PlanExecutionState::PES_Succeed;
}

bool USP_TargetTask::PreCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_PRECONDITION(PreCondition, Planner, PlannerFlags)

	return PreCondition_Internal(Planner, Planner->GetTarget(), PlannerFlags);
}
int USP_TargetTask::PostCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const
{
	SP_TASK_SUPER_POSTCONDITION(PostCondition, Planner, PlannerFlags)

	return PostCondition_Internal(Planner, Planner->GetTarget(), PlannerFlags);
}

ESP_PlanExecutionState USP_TargetTask::Begin(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Begin, Planner, UserData)

	return Begin_Internal(Planner, Planner->GetTarget(), UserData);
}
ESP_PlanExecutionState USP_TargetTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	return Tick_Internal(DeltaSeconds, Planner, Planner->GetTarget(), UserData);
}
ESP_PlanExecutionState USP_TargetTask::End(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(End, Planner, UserData)

	return End_Internal(Planner, Planner->GetTarget(), UserData);
}