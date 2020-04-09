// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Task/Impl/SP_ParallelTask.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_ParallelTask::USP_ParallelTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_ParallelTaskInfos::StaticClass();
}

bool USP_ParallelTask::PreCondition_Implementation(const USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Infos)

	SP_RCHECK_NULLPTR(MainTask, false)

	if (!MainTask->PreCondition(Infos))
		return false;

	if (bUseSubCheck)
	{
		SP_RCHECK_NULLPTR(SubTask, false)

		if (!SubTask->PreCondition(Infos))
			return false;
	}

	return true;
}
bool USP_ParallelTask::PostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Infos)

	SP_RCHECK_NULLPTR(MainTask, false)

	if (!MainTask->PostCondition(Infos))
		return false;

	if (bUseSubCheck)
	{
		SP_RCHECK_NULLPTR(SubTask, false)

		if (!SubTask->PostCondition(Infos))
			return false;
	}

	return true;
}
bool USP_ParallelTask::ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Infos)

	SP_RCHECK_NULLPTR(MainTask, false)

	if (!MainTask->ResetPostCondition(Infos))
		return false;

	if (bUseSubCheck)
	{
		SP_RCHECK_NULLPTR(SubTask, false)

		if (!SubTask->ResetPostCondition(Infos))
			return false;
	}


	return true;
}

bool USP_ParallelTask::IsAvailable(const USP_PlannerComponent* Planner) const
{
	if (!Super::IsAvailable(Planner))
		return false;

	SP_RCHECK_NULLPTR(MainTask, false)

	if (!MainTask->IsAvailable(Planner))
		return false;

	if(bUseSubCheck)
	{
		SP_RCHECK_NULLPTR(SubTask, false)

		if(!SubTask->IsAvailable(Planner))
			return false;
	}

	return true;
}
void USP_ParallelTask::PostExecution(const USP_PlannerComponent* Planner, bool bExecutionSuccess)
{
	Super::PostExecution(Planner, bExecutionSuccess);

	SP_CHECK_NULLPTR(MainTask)
	MainTask->PostExecution(Planner, bExecutionSuccess);

	// SubTask PostExecution handled by End().
}

void USP_ParallelTask::OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos)
{
	Super::OnNotify(Planner, Notify, TaskInfos);

	USP_ParallelTaskInfos* const Infos = Cast<USP_ParallelTaskInfos>(TaskInfos);
	SP_CHECK(Infos, "Infos nullptr! TaskInfos must be of type USP_ParallelTaskInfos")

	SP_CHECK_NULLPTR(MainTask)
	MainTask->OnNotify(Planner, Notify, Infos->MainTaskInfos);

	// Sub task being executed
	if (Infos->SubTaskInfos)
	{
		SP_CHECK_NULLPTR(SubTask)
		SubTask->OnNotify(Planner, Notify, Infos->SubTaskInfos);
	}
}

bool USP_ParallelTask::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_ParallelTaskInfos* const Infos = Cast<USP_ParallelTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_ParallelTaskInfos")

	SP_RCHECK_NULLPTR(MainTask, false)
	Infos->MainTaskInfos = MainTask->InstantiateInfos();
	Infos->MainTaskInfos->bUseNotify = false; // ParallelTask handle notify forwarding.

	SP_RCHECK_NULLPTR(SubTask, false)
	Infos->SubTaskInfos = SubTask->InstantiateInfos();
	Infos->SubTaskInfos->bUseNotify = false; // ParallelTask handle notify forwarding.

	return true;
}
ESP_PlanExecutionState USP_ParallelTask::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_ParallelTaskInfos* const Infos = Cast<USP_ParallelTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_ParallelTaskInfos")

	SP_RCHECK_NULLPTR(MainTask, ESP_PlanExecutionState::PES_Failed)
	ESP_PlanExecutionState MainTickResult = MainTask->Tick(DeltaSeconds, *Planner, Infos->MainTaskInfos);

	// Main ended.
	if (MainTickResult != ESP_PlanExecutionState::PES_Running)
		return MainTickResult;


	// Sub task being executed
	if (Infos->SubTaskInfos)
	{
		SP_RCHECK_NULLPTR(SubTask, ESP_PlanExecutionState::PES_Failed)
		ESP_PlanExecutionState SubTickResult = SubTask->Tick(DeltaSeconds, *Planner, Infos->SubTaskInfos);

		if (SubTickResult != ESP_PlanExecutionState::PES_Running)
		{
			// Execution end.
			Infos->SubTaskInfos = nullptr;
			SubTask->PostExecution(Planner, SubTickResult == ESP_PlanExecutionState::PES_Succeed);

			// Restart
			if (bLoopSubTask && SubTickResult == ESP_PlanExecutionState::PES_Succeed)
			{
				Infos->SubTaskInfos = SubTask->InstantiateInfos();
				Infos->SubTaskInfos->bUseNotify = false; // ParallelTask handle notify forwarding.
			}
		}
	}

	return ESP_PlanExecutionState::PES_Running;
}
bool USP_ParallelTask::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_END(Planner, TaskInfos)

	USP_ParallelTaskInfos* const Infos = Cast<USP_ParallelTaskInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_ParallelTaskInfos")

	// Sub task being executed
	if (Infos->SubTaskInfos)
	{
		SP_RCHECK_NULLPTR(SubTask, false)
		SubTask->Cancel(*Planner, Infos->SubTaskInfos);
	}

	return true;
}