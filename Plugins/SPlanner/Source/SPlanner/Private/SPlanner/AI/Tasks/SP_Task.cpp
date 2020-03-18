// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/SP_Task.h>

#include <SPlanner/Miscs/SP_FlagHelper.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>

USP_Task::USP_Task(const FObjectInitializer& ObjectInitializer)
{
	// Default Cooldown is 0.
	Cooldown.Default = 0.0f;

	// Default never TimeOut.
	TimeOut.Default = -1.0f;

	TaskInfosClass = USP_TaskInfos::StaticClass();
}

bool USP_Task::GetUseCooldownOnFailed() const
{
	return bUseCooldownOnFailed;
}
float USP_Task::GetCooldown(float LODLevel) const
{
	return Cooldown.Get(LODLevel);
}

void USP_Task::OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK_NULLPTR(TaskInfos)

	if (!SP_IS_FLAG_SET(NotifyMask, Notify))
		return;

	switch (NotifyAction)
	{
	case ESP_TaskNotification::TNA_TimeOut:
		TaskInfos->BaseExecutionState = ESP_PlanExecutionState::PES_Failed;
		break;
	case ESP_TaskNotification::TNA_WaitForNotify:
		TaskInfos->BaseExecutionState = ESP_PlanExecutionState::PES_Succeed;
		break;
	default:
		SP_LOG(Warning, "TaskNotification [%d] not supported yet!", static_cast<int>(NotifyAction))
		break;
	}
}
void USP_Task::InitNotify(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(Planner)

	// Always bind to allow OnNotify() override.
	Planner->OnNotifyTask.AddDynamic(this, &USP_Task::OnNotify);

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
		SP_LOG(Warning, "TaskNotification [%d] not supported yet!", static_cast<int>(NotifyAction))
		break;
	}
}

USP_TaskInfos* USP_Task::InstantiateInfos(UObject* Outer)
{
	SP_RCHECK_NULLPTR(Outer, nullptr)
	SP_RCHECK_NULLPTR(TaskInfosClass, nullptr)

	return NewObject<USP_TaskInfos>(Outer, TaskInfosClass);
}

bool USP_Task::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlanGenInfos)

	SP_RCHECK(Cast<USP_AIPlannerComponent>(Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")
	SP_RCHECK(Cast<USP_AIPlanGenInfos>(PlanGenInfos), false, "AI PlanGenInfos nullptr! PlanGenInfos must be a USP_AIPlanGenInfos!")

	return true;
}
bool USP_Task::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	SP_RCHECK(Cast<USP_AIPlannerComponent>(Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")
	SP_RCHECK(Cast<USP_AIPlanGenInfos>(PlanGenInfos), false, "AI PlanGenInfos nullptr! PlanGenInfos must be a USP_AIPlanGenInfos!")

	return true;
}
bool USP_Task::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	SP_RCHECK(Cast<USP_AIPlannerComponent>(Planner), false, "AI Planner nullptr! Planner must be a USP_AIPlannerComponent!")
	SP_RCHECK(Cast<USP_AIPlanGenInfos>(PlanGenInfos), false, "AI PlanGenInfos nullptr! PlanGenInfos must be a USP_AIPlanGenInfos!")

	return true;
}

ESP_PlanExecutionState USP_Task::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(TaskInfos, ESP_PlanExecutionState::PES_Failed)

	// Begin Task.
	if (!TaskInfos->bHasBegun && !Begin_Internal(&Planner, TaskInfos))
	{
		// Begin has failed.
		End_Internal(&Planner, TaskInfos); // Call End to uninit.

		return ESP_PlanExecutionState::PES_Failed;
	}

	ESP_PlanExecutionState InternalState = Tick_Internal(DeltaSeconds, &Planner, TaskInfos);

	// Wait for success or failure.
	if (InternalState == ESP_PlanExecutionState::PES_Running)
		return InternalState;
	
	// End Task.
	if (!End_Internal(&Planner, TaskInfos))
		return ESP_PlanExecutionState::PES_Failed; // End has failed.

	// Return either tick has succeeded or failed.
	return InternalState;
}
void USP_Task::Cancel(USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos)
{
	SP_CHECK_NULLPTR(TaskInfos)

	if (TaskInfos->bHasBegun)
	{
		TaskInfos->bForcedEnd = true;
		End_Internal(&Planner, TaskInfos);
	}
}

bool USP_Task::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)
	SP_RCHECK_NULLPTR(TaskInfos, false)

	// Time Out.
	float TimeOutTime = TimeOut.Get(Planner->GetLODLevel());

	if (TimeOutTime > 0.0f)
		TaskInfos->TimeOutTime = TimeOutTime;

	if(TaskInfos->bUseNotify)
		InitNotify(Planner, TaskInfos);

	TaskInfos->bHasBegun = true;

	return true;
}
ESP_PlanExecutionState USP_Task::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(TaskInfos, ESP_PlanExecutionState::PES_Failed)

	// Time Out.
	if (TaskInfos->TimeOutTime > 0.0f)
	{
		TaskInfos->CurrTimeOut += DeltaSeconds;

		if (TaskInfos->CurrTimeOut >= TaskInfos->TimeOutTime)
			return ESP_PlanExecutionState::PES_Failed;
	}


	return TaskInfos->BaseExecutionState;
}
bool USP_Task::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)

	Planner->OnNotifyTask.RemoveDynamic(this, &USP_Task::OnNotify);

	return true;
}