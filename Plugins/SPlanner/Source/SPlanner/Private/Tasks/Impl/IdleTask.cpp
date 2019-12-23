#include <Tasks/Impl/IdleTask.h>

uint32 USP_IdleTask::GetUserDataSize() const
{
	return sizeof(FTaskInfos);
}

ESP_PlanExecutionState USP_IdleTask::Begin(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Begin, Planner, UserData)

	FTaskInfos* Infos = new(UserData) FTaskInfos{ FMath::RandRange(MinTime, MaxTime) };

	SP_LOG_EXECUTE(Planner->GetOwner(), "%f", Infos->WaitTime)

	return ESP_PlanExecutionState::PES_Succeed;
}

ESP_PlanExecutionState USP_IdleTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	FTaskInfos* Infos = reinterpret_cast<FTaskInfos*>(UserData);

	Infos->CurrTime += DeltaSeconds;

	if (Infos->CurrTime >= Infos->WaitTime)
		return ESP_PlanExecutionState::PES_Succeed;

	return ESP_PlanExecutionState::PES_Running;
}

ESP_PlanExecutionState USP_IdleTask::End(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(End, Planner, UserData)

	reinterpret_cast<FTaskInfos*>(UserData)->~FTaskInfos();

	return ESP_PlanExecutionState::PES_Succeed;
}