#include <SPlanner/Actions/Tasks/SP_Task.h>

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

bool USP_Task::Begin(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, false)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnBegin(Planner);
#endif

	return true;
}
ESP_PlanExecutionState USP_Task::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnTick(DeltaSeconds, Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}
bool USP_Task::End(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, false)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnEnd(Planner);
#endif

	return true;
}

bool USP_Task::Cancel(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, false)

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
		OnCancel(Planner);
#endif

	return true;
}