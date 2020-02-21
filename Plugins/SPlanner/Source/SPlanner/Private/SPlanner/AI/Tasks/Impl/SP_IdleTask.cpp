// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_IdleTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_TaskInfosBase* USP_IdleTask::InstantiateInfos()
{
	return NewObject<USP_IdleTaskInfos>();
}

bool USP_IdleTask::Begin(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_IdleTaskInfos* const Infos = Cast<USP_IdleTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_IdleTaskInfos")

	Infos->WaitTime = FMath::RandRange(MinTime, MaxTime);

	SP_LOG_TASK_EXECUTE(Planner, "%f", Infos->WaitTime)

	return true;
}
ESP_PlanExecutionState USP_IdleTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_IdleTaskInfos* const Infos = Cast<USP_IdleTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_IdleTaskInfos")

	Infos->CurrTime += DeltaSeconds;

	if (Infos->CurrTime >= Infos->WaitTime)
		return ESP_PlanExecutionState::PES_Succeed;

	return ESP_PlanExecutionState::PES_Running;
}