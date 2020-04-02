// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleBaseTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_IdleBaseTask::USP_IdleBaseTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_IdleBaseTaskInfos::StaticClass();
}

float USP_IdleBaseTask::GenerateTime_Implementation(const USP_AIPlannerComponent* Planner)
{
	SP_LOG(Error, "Method must be overridden in children!")

	return 0.0f;
}

bool USP_IdleBaseTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_IdleBaseTaskInfos* const Infos = Cast<USP_IdleBaseTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_IdleBaseTaskInfos")

	Infos->WaitTime = GenerateTime(Planner);

	SP_LOG_TASK_EXECUTE(Planner, "%f", Infos->WaitTime)

	return true;
}
ESP_PlanExecutionState USP_IdleBaseTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_IdleBaseTaskInfos* const Infos = Cast<USP_IdleBaseTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_IdleBaseTaskInfos")

	Infos->CurrTime += DeltaSeconds;

	if (Infos->CurrTime >= Infos->WaitTime)
		return ESP_PlanExecutionState::PES_Succeed;

	return ESP_PlanExecutionState::PES_Running;
}