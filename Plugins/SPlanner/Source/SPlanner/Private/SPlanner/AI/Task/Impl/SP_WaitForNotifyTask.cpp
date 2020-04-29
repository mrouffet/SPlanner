// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include "SP_WaitForNotifyTask.h"

USP_WaitForNotifyTask::USP_WaitForNotifyTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_WaitForNotifyTaskInfos::StaticClass();
}

void USP_WaitForNotifyTask::Notify()
{
	for (int i = 0; i < CachedInfos.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(CachedInfos[i])

		CachedInfos[i]->NotifyExecutionState = ESP_PlanExecutionState::PES_Succeed;
	}
}

bool USP_WaitForNotifyTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_WaitForNotifyTaskInfos* Infos = Cast<USP_WaitForNotifyTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_WaitForNotifyTask")

	CachedInfos.Add(Infos);

	return true;
}
ESP_PlanExecutionState USP_WaitForNotifyTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_WaitForNotifyTaskInfos* Infos = Cast<USP_WaitForNotifyTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_WaitForNotifyTask")

	return Infos->NotifyExecutionState;
}
bool USP_WaitForNotifyTask::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_END(Planner, TaskInfos)

	USP_WaitForNotifyTaskInfos* Infos = Cast<USP_WaitForNotifyTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_WaitForNotifyTask")

	CachedInfos.Remove(Infos);

	return true;
}