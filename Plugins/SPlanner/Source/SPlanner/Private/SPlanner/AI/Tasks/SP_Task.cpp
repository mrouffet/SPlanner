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
		SP_LOG(Warning, "TaskNotification not supported yet!")
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
	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	// Begin Task.
	if (!Infos->bHasBegun && !Begin_Internal(&Planner, TaskInfos))
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
	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_CHECK(Infos, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	if (Infos->bHasBegun)
	{
		Infos->bForcedEnd = true;
		End_Internal(&Planner, TaskInfos);
	}
}

bool USP_Task::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)

	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	// Time Out.
	float TimeOutTime = TimeOut.Get(Planner->GetLODLevel());

	if (TimeOutTime > 0.0f)
		Infos->TimeOutTime = TimeOutTime;


	InitNotify(Planner, Infos);

	Infos->bHasBegun = true;

	return true;
}
ESP_PlanExecutionState USP_Task::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, ESP_PlanExecutionState::PES_Failed)

	USP_TaskInfos* const Infos = Cast<USP_TaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_TaskInfos")

	// Time Out.
	if (Infos->TimeOutTime > 0.0f)
	{
		Infos->CurrTimeOut += DeltaSeconds;

		if (Infos->CurrTimeOut >= Infos->TimeOutTime)
			return ESP_PlanExecutionState::PES_Failed;
	}


	return Infos->BaseExecutionState;
}
bool USP_Task::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_RCHECK_NULLPTR(Planner, false)

	Planner->OnNotifyTask.RemoveDynamic(this, &USP_Task::OnNotify);

	return true;
}