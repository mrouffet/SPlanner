// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/SP_Task.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_Task::USP_Task(const FObjectInitializer& ObjectInitializer)
{
	// Default Cooldown is 0.
	Cooldown.Default = 0.0f;

	// Default never TimeOut.
	TimeOut.Default = -1.0f;
}

bool USP_Task::GetUseCooldownOnFailed() const
{
	return bUseCooldownOnFailed;
}
float USP_Task::GetCooldown(float LODLevel) const
{
	return Cooldown.Get(LODLevel);
}

uint32 USP_Task::GetUserDataSize() const
{
	return sizeof(FSP_TaskInfos);
}

bool USP_Task::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	SP_RCHECK_NULLPTR(Cast<USP_AIPlannerComponent>(&Planner), false)

	return true;
}

void USP_Task::ConstructUserData(uint8* UserData)
{
	new(UserData) FSP_TaskInfos();
}
void USP_Task::DestructUserData(uint8* UserData)
{
	reinterpret_cast<FSP_TaskInfos*>(UserData)->~FSP_TaskInfos();
}

bool USP_Task::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	ConstructUserData(UserData);

	// Time Out.
	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	float TimeOutTime = TimeOut.Get(Planner.GetLODLevel());

	if (TimeOutTime > 0.0f)
		Infos->T_TimeOutTime = TimeOutTime;


#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskBegin(&Planner);
#endif

	return true;
}
ESP_PlanExecutionState USP_Task::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	FSP_TaskInfos* const Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	// Time Out.
	if (Infos->T_TimeOutTime > 0.0f)
	{
		Infos->T_CurrTimeOut += DeltaSeconds;

		if (Infos->T_CurrTimeOut >= Infos->T_TimeOutTime)
			return ESP_PlanExecutionState::PES_Failed;
	}


#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskTick(DeltaSeconds, &Planner);
#endif

	return ESP_PlanExecutionState::PES_Succeed;
}
bool USP_Task::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	DestructUserData(UserData);

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskEnd(&Planner);
#endif

	return true;
}

bool USP_Task::Cancel(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	DestructUserData(UserData);

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