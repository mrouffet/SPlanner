#include <Actions/Tasks/SP_TaskChain.h>

uint32 USP_TaskChain::GetUserDataSize() const
{
	return sizeof(FSP_TaskInfos);
}

bool USP_TaskChain::PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, PlannerFlags)

	SP_RCHECK(Tasks.Num(), "Empty tasks!", false)

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], false)

		if (!Tasks[i]->PreCondition(Planner, PlannerFlags))
			return false;

		PlannerFlags = Tasks[i]->PostCondition(Planner, PlannerFlags);
	}

	return true;
}
FSP_PlannerFlags USP_TaskChain::PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlannerFlags)

	SP_RCHECK(Tasks.Num(), "Empty tasks!", PlannerFlags)

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], PlannerFlags)

		// Precondition has already been checked.
		PlannerFlags = Tasks[i]->PostCondition(Planner, PlannerFlags);
	}

	return PlannerFlags;
}

ESP_PlanExecutionState USP_TaskChain::Begin(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Begin, Planner, UserData)

	FSP_TaskInfos* Infos = new (UserData) FSP_TaskInfos();

	SP_RCHECK(Tasks.Num(), "Empty tasks!", ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(Tasks[0], ESP_PlanExecutionState::PES_Failed)

	if(Tasks[0]->GetUserDataSize())
		Infos->UserData.Reserve(Tasks[0]->GetUserDataSize());

	return Tasks[0]->Begin(Planner, Infos->UserData.GetData());
}
ESP_PlanExecutionState USP_TaskChain::Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(Tick, DeltaSeconds, Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	SP_RCHECK(Infos->Index < Tasks.Num(), "Index out of range!", ESP_PlanExecutionState::PES_Failed)
	SP_RCHECK_NULLPTR(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed)

	// Tick current task.
	ESP_PlanExecutionState TickResult = Tasks[Infos->Index]->Tick(DeltaSeconds, Planner, Infos->UserData.GetData());

	if (TickResult != ESP_PlanExecutionState::PES_Succeed)
		return TickResult; // Failed or rurnning.

	// End task.
	ESP_PlanExecutionState EndResult = Tasks[Infos->Index]->End(Planner, Infos->UserData.GetData());

	if (EndResult != ESP_PlanExecutionState::PES_Succeed)
		return ESP_PlanExecutionState::PES_Failed;

	// All task ended with success.
	if (++Infos->Index == Tasks.Num())
		return ESP_PlanExecutionState::PES_Succeed;

	// Begin next task.
	SP_RCHECK_NULLPTR(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed)

	if (Tasks[Infos->Index]->GetUserDataSize())
		Infos->UserData.Reserve(Tasks[Infos->Index]->GetUserDataSize());

	ESP_PlanExecutionState BeginResult = Tasks[Infos->Index]->Begin(Planner, Infos->UserData.GetData());

	if(BeginResult != ESP_PlanExecutionState::PES_Succeed)
		return ESP_PlanExecutionState::PES_Failed;

	return ESP_PlanExecutionState::PES_Running;
}
ESP_PlanExecutionState USP_TaskChain::End(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	SP_TASK_SUPER(End, Planner, UserData)

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	ESP_PlanExecutionState Result = ESP_PlanExecutionState::PES_Succeed;

	// Task has failed.
	if (Infos->Index < Tasks.Num())
	{
		SP_RCHECK_NULLPTR(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed)
		Result = Tasks[Infos->Index]->End(Planner, Infos->UserData.GetData()); // end failed task.
	}

	Infos->~FSP_TaskInfos();

	return Result;
}

bool USP_TaskChain::Cancel(USP_AIPlannerComponent* Planner, uint8* UserData)
{
	if (!Super::Cancel(Planner, UserData))
		return false;

	FSP_TaskInfos* Infos = reinterpret_cast<FSP_TaskInfos*>(UserData);

	bool Result = true;

	// Task was executing.
	if (Infos->Index < Tasks.Num())
	{
		SP_RCHECK_NULLPTR(Tasks[Infos->Index], false)
		Result = Tasks[Infos->Index]->Cancel(Planner, Infos->UserData.GetData());
	}

	Infos->~FSP_TaskInfos();

	return Result;
}