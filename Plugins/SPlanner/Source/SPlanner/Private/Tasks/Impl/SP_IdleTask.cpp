#include <Tasks/Impl/SP_IdleTask.h>

#include <Components/SP_PlannerComponent.h>

uint32 USP_IdleTask::GetUserDataSize() const
{
	return sizeof(FSP_TaskInfos);
}

ESP_PlanExecutionState USP_IdleTask::Begin(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Begin, Planner, UserData)

	FSP_TaskInfos* Infos = new(UserData) FSP_TaskInfos{ FMath::RandRange(MinTime, MaxTime) };

	SP_LOG_TASK_EXECUTE(Planner->GetOwner(), "%f", Infos->WaitTime)

	return ESP_PlanExecutionState::PES_Succeed;
}
ESP_PlanExecutionState USP_IdleTask::Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	Infos->CurrTime += DeltaSeconds;

	if (Infos->CurrTime >= Infos->WaitTime)
		return ESP_PlanExecutionState::PES_Succeed;

	return ESP_PlanExecutionState::PES_Running;
}
ESP_PlanExecutionState USP_IdleTask::End(USP_PlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(End, Planner, UserData)

	reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();

	return ESP_PlanExecutionState::PES_Succeed;
}