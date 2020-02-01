#include <SPlanner/AI/Tasks/Impl/SP_IdleTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

uint32 USP_IdleTask::GetUserDataSize() const
{
	return sizeof(FSP_TaskInfos);
}

bool USP_IdleTask::Begin(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_AI_TASK_BEGIN_SUPER(Planner, UserData)

	FSP_TaskInfos* Infos = new(UserData) FSP_TaskInfos{ FMath::RandRange(MinTime, MaxTime) };

	SP_LOG_AI_TASK_EXECUTE(Planner, "%f", Infos->WaitTime)

	return true;
}
ESP_PlanExecutionState USP_IdleTask::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_AI_TASK_TICK_SUPER(DeltaSeconds, Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	Infos->CurrTime += DeltaSeconds;

	if (Infos->CurrTime >= Infos->WaitTime)
		return ESP_PlanExecutionState::PES_Succeed;

	return ESP_PlanExecutionState::PES_Running;
}
bool USP_IdleTask::End(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_AI_TASK_END_SUPER(Planner, UserData)

	reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();

	return true;
}