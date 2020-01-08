#include <Tasks/Task.h>

bool USP_Task::GetUseCooldownOnFailed() const
{
	return bUseCooldownOnFailed;
}

float USP_Task::GetCooldown() const
{
	return Cooldown;
}

uint32 USP_Task::GetUserDataSize() const
{
	return 0u;
}

bool USP_Task::PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return true;
}
FSP_PlannerFlags USP_Task::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_RCHECK_NULLPTR(Planner, PlannerFlags)

	return PlannerFlags;
}

ESP_PlanExecutionState USP_Task::Begin(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnBegin(Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}

ESP_PlanExecutionState USP_Task::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnTick(DeltaSeconds, Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}

ESP_PlanExecutionState USP_Task::End(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnEnd(Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}