#include <SPlanner/AI/Tasks/SP_Task.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

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

bool USP_Task::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	SP_RCHECK_NULLPTR(Cast<USP_AIPlannerComponent>(&Planner), false)

	return true;
}

bool USP_Task::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskBegin(&Planner);
#endif

	return true;
}
ESP_PlanExecutionState USP_Task::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskTick(DeltaSeconds, &Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}
bool USP_Task::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskEnd(&Planner);
#endif

	return true;
}

bool USP_Task::Cancel(USP_AIPlannerComponent& Planner, uint8* UserData)
{
#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskCancel(&Planner);
#endif

	return true;
}

bool USP_Task::K2_OnTaskBegin_Implementation(USP_AIPlannerComponent* Planner)
{
	return true;
}
ESP_PlanExecutionState USP_Task::K2_OnTaskTick_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner)
{
	return ESP_PlanExecutionState::PES_Succeed;
}
bool USP_Task::K2_OnTaskEnd_Implementation(USP_AIPlannerComponent* Planner)
{
	return true;
}

bool USP_Task::K2_OnTaskCancel_Implementation(USP_AIPlannerComponent* Planner)
{
	return true;
}