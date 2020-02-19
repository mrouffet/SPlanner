// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/SP_TaskChain.h>

uint32 USP_TaskChain::GetUserDataSize() const
{
	uint32 MaxSize = 0;

	// Query biggest user data size.
	for (int i = 0; i < Tasks.Num(); ++i)
	{
		if(Tasks[i]->GetUserDataSize() > MaxSize)
			MaxSize = Tasks[i]->GetUserDataSize();
	}

	return MaxSize + sizeof(FSP_TaskInfos);
}

bool USP_TaskChain::PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlannerFlags)

	SP_RCHECK(Tasks.Num(), false, "Empty tasks!")

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], false)

		if (!Tasks[i]->PreCondition(Planner, GeneratedPlan, PlannerFlags))
			return false;

		PlannerFlags = Tasks[i]->PostCondition(Planner, PlannerFlags);
	}

	return true;
}
uint64 USP_TaskChain::PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_RCHECK(Tasks.Num(), PlannerFlags, "Empty tasks!")

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], PlannerFlags)

		// Precondition has already been checked.
		PlannerFlags = Tasks[i]->PostCondition(Planner, PlannerFlags);
	}

	return PlannerFlags;
}

bool USP_TaskChain::Begin(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_BEGIN(Planner, UserData)

	FSP_TaskInfos* Infos = new (UserData) FSP_TaskInfos();

	SP_RCHECK(Tasks.Num(), false, "Empty tasks!")
	SP_RCHECK_NULLPTR(Tasks[0], false)

	return Tasks[0]->Begin(Planner, UserData + sizeof(FSP_TaskInfos));
}
ESP_PlanExecutionState USP_TaskChain::Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	SP_RCHECK(Infos->Index < Tasks.Num(), ESP_PlanExecutionState::PES_Failed, "Index out of range!")
	SP_RCHECK_NULLPTR(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed)

	// Tick current task.
	ESP_PlanExecutionState TickResult = Tasks[Infos->Index]->Tick(DeltaSeconds, Planner, UserData + sizeof(FSP_TaskInfos));

	if (TickResult != ESP_PlanExecutionState::PES_Succeed)
		return TickResult; // Failed or rurnning.


	// End task.
	if (!Tasks[Infos->Index]->End(Planner, UserData + sizeof(FSP_TaskInfos)))
		return ESP_PlanExecutionState::PES_Failed;

	// All task ended with success.
	if (++Infos->Index == Tasks.Num())
		return ESP_PlanExecutionState::PES_Succeed;


	// Begin next task.
	SP_RCHECK_NULLPTR(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed)

	// Begin task.
	if(!Tasks[Infos->Index]->Begin(Planner, UserData + sizeof(FSP_TaskInfos)))
		return ESP_PlanExecutionState::PES_Failed;

	return ESP_PlanExecutionState::PES_Running;
}
bool USP_TaskChain::End(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_END(Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	bool Result = true;

	// Task has failed.
	if (Infos->Index < Tasks.Num())
	{
		SP_RCHECK_NULLPTR(Tasks[Infos->Index], false)
		Result = Tasks[Infos->Index]->End(Planner, UserData + sizeof(FSP_TaskInfos)); // end failed task.
	}

	Infos->~FSP_TaskInfos();

	return Result;
}

bool USP_TaskChain::Cancel(USP_AIPlannerComponent& Planner, uint8* UserData)
{
	SP_TASK_SUPER_CANCEL(Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	bool Result = true;

	// Task was executing.
	if (Infos->Index < Tasks.Num())
	{
		SP_RCHECK_NULLPTR(Tasks[Infos->Index], false)
		Result = Tasks[Infos->Index]->Cancel(Planner, UserData + sizeof(FSP_TaskInfos));
	}

	Infos->~FSP_TaskInfos();

	return Result;
}