// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/SP_Task.h>

#include <SPlanner/Miscs/SP_FlagHelper.h>

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

void USP_Task::OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfosBase* TaskInfos)
{
	SP_CHECK_NULLPTR(Planner)

	if (!SP_IS_FLAG_SET(NotifyMask, Notify))
		return;

	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_CHECK(Infos, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	switch (NotifyAction)
	{
	case ESP_TaskNotification::TNA_TimeOut:
		Infos->BaseExecutionState = ESP_PlanExecutionState::PES_Failed;
		break;
	case ESP_TaskNotification::TNA_WaitForNotify:
		Infos->BaseExecutionState = ESP_PlanExecutionState::PES_Succeed;
		break;
	default:
		SP_LOG(Warning, "TaskNotification not supported yet!")
		break;
	}
}

void USP_Task::InitNotify(USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
{
	// Always bind to allow OnNotify() override.
	Planner.OnNotifyTask.AddDynamic(this, &USP_Task::OnNotify);

	if (!NotifyMask)
		return;

	switch (NotifyAction)
	{
	case ESP_TaskNotification::TNA_TimeOut:
		break;
	case ESP_TaskNotification::TNA_WaitForNotify:
		TaskInfos->BaseExecutionState = ESP_PlanExecutionState::PES_Running;
		break;
	default:
		SP_LOG(Warning, "TaskNotification not supported yet!")
		break;
	}
}

USP_TaskInfosBase* USP_Task::InstantiateInfos()
{
	return NewObject<USP_TaskInfos>();
}

bool USP_Task::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	SP_RCHECK_NULLPTR(Cast<USP_AIPlannerComponent>(&Planner), false)

	return true;
}

bool USP_Task::Begin(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos)
{
	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	// Time Out.
	float TimeOutTime = TimeOut.Get(Planner.GetLODLevel());

	if (TimeOutTime > 0.0f)
		Infos->TimeOutTime = TimeOutTime;


	InitNotify(Planner, Infos);

#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskBegin(&Planner);
#endif

	return true;
}
ESP_PlanExecutionState USP_Task::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos)
{
	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	// Time Out.
	if (Infos->TimeOutTime > 0.0f)
	{
		Infos->CurrTimeOut += DeltaSeconds;

		if (Infos->CurrTimeOut >= Infos->TimeOutTime)
			return ESP_PlanExecutionState::PES_Failed;
	}


#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	ESP_PlanExecutionState BPExecutionState = K2_OnTaskTick(DeltaSeconds, &Planner);
	
	if (BPExecutionState != ESP_PlanExecutionState::PES_Succeed)
		return BPExecutionState;
#endif

	return Infos->BaseExecutionState;
}
bool USP_Task::End(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos)
{
	Planner.OnNotifyTask.RemoveDynamic(this, &USP_Task::OnNotify);


#if SP_TASK_BLUEPRINT_IMPLEMENTABLE
	return K2_OnTaskEnd(&Planner);
#endif

	return true;
}

bool USP_Task::Cancel(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos)
{
	Planner.OnNotifyTask.RemoveDynamic(this, &USP_Task::OnNotify);


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