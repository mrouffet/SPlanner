#include <SPlanner/Actions/AITasks/SP_AITask.h>

#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>

bool USP_AITask::GetUseCooldownOnFailed() const
{
	return bUseCooldownOnFailed;
}
float USP_AITask::GetCooldown() const
{
	return Cooldown;
}

uint32 USP_AITask::GetUserDataSize() const
{
	return 0u;
}

bool USP_AITask::PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, PlannerFlags)

	SP_RCHECK_NULLPTR(Cast<USP_AIPlannerComponent>(Planner), false)

	return true;
}

bool USP_AITask::Begin(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, false)

#if SP_AI_TASK_BLUEPRINT_IMPLEMENTABLE
	OnBegin(Planner);
#endif

	return true;
}
ESP_PlanExecutionState USP_AITask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)

#if SP_AI_TASK_BLUEPRINT_IMPLEMENTABLE
	OnTick(DeltaSeconds, Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}
bool USP_AITask::End(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, false)

#if SP_AI_TASK_BLUEPRINT_IMPLEMENTABLE
	OnEnd(Planner);
#endif

	return true;
}

bool USP_AITask::Cancel(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_RCHECK_NULLPTR(Planner, false)

#if SP_AI_TASK_BLUEPRINT_IMPLEMENTABLE
	OnCancel(Planner);
#endif

	return true;
}