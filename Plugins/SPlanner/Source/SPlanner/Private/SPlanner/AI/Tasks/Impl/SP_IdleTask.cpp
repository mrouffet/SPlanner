// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/Impl/SP_IdleTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

uint32 USP_IdleTask::GetUserDataSize() const
{
	return sizeof(FSP_IdleTaskInfos);
}

void USP_IdleTask::ConstructUserData(uint8* UserData)
{
	new(UserData) FSP_IdleTaskInfos();
}
void USP_IdleTask::DestructUserData(uint8* UserData)
{
	reinterpret_cast<FSP_IdleTaskInfos*>(UserData)->~FSP_IdleTaskInfos();
}

bool USP_IdleTask::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_BEGIN(Planner, UserData)

	FSP_IdleTaskInfos* const Infos = reinterpret_cast<FSP_IdleTaskInfos*>(UserData);
	Infos->IT_WaitTime = FMath::RandRange(MinTime, MaxTime);

	SP_LOG_TASK_EXECUTE(Planner, "%f", Infos->IT_WaitTime)

	return true;
}
ESP_PlanExecutionState USP_IdleTask::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, UserData)

	FSP_IdleTaskInfos* const Infos = reinterpret_cast<FSP_IdleTaskInfos*>(UserData);

	Infos->IT_CurrTime += DeltaSeconds;

	if (Infos->IT_CurrTime >= Infos->IT_WaitTime)
		return ESP_PlanExecutionState::PES_Succeed;

	return ESP_PlanExecutionState::PES_Running;
}