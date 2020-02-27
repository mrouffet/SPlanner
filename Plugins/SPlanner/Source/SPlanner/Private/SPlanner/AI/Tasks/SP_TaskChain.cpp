// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Tasks/SP_TaskChain.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

USP_TaskChain::USP_TaskChain(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskInfosClass = USP_TaskChainInfos::StaticClass();
}

void USP_TaskChain::OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos)
{
	Super::OnNotify(Planner, Notify, TaskInfos);

	USP_TaskChainInfos* const Infos = Cast<USP_TaskChainInfos>(TaskInfos);
	SP_CHECK(Infos, "Infos nullptr! TaskInfos must be of type USP_TaskChainInfos")

	Tasks[Infos->Index]->OnNotify(Planner, Notify, Infos->TaskInfos);
}

bool USP_TaskChain::PreCondition_Implementation(const USP_PlannerComponent* Planner,
	const TArray<USP_ActionStep*>& GeneratedPlan,
	const USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_PRECONDITION(Planner, GeneratedPlan, PlanGenInfos)

	SP_RCHECK(Tasks.Num(), false, "Empty tasks!")

	/**
	*	PostCondition will modify PlanGenInfos.
	*	PostCondition must be added to check next PreConditions.
	*	Call ResetPostCondition to get back initial PlanGenInfos.
	*	Avoid PlanGenInfos copy.
	*/
	USP_PlanGenInfos* NonConstPlanGenInfos = const_cast<USP_PlanGenInfos*>(PlanGenInfos);

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], false)

		if (!Tasks[i]->PreCondition(Planner, GeneratedPlan, PlanGenInfos) || !Tasks[i]->PostCondition(Planner, NonConstPlanGenInfos))
		{
			// PostCondition failed: Reset all previously set.

			for (int j = i - 1; j >= 0; --j)
				Tasks[j]->ResetPostCondition(Planner, NonConstPlanGenInfos);

			return false;
		}
	}


	// Reset NonConstPlanGenInfos to initial value.
	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], false)

		Tasks[i]->ResetPostCondition(Planner, NonConstPlanGenInfos);
	}

	return true;
}
bool USP_TaskChain::PostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_POSTCONDITION(Planner, PlanGenInfos)

	SP_RCHECK(Tasks.Num(), false, "Empty tasks!")

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], false)

		// Precondition has already been checked.
		if (!Tasks[i]->PostCondition(Planner, PlanGenInfos))
		{
			// Post Condition failed: Reset all previously set.

			for (int j = i - 1; j >= 0; --j)
				Tasks[j]->ResetPostCondition(Planner, PlanGenInfos);

			return false;
		}
	}

	return true;
}
bool USP_TaskChain::ResetPostCondition_Implementation(const USP_PlannerComponent* Planner, USP_PlanGenInfos* PlanGenInfos) const
{
	SP_ACTION_STEP_SUPER_RESET_POSTCONDITION(Planner, PlanGenInfos)

	SP_RCHECK(Tasks.Num(), false, "Empty tasks!")

	for (int i = 0; i < Tasks.Num(); ++i)
	{
		SP_RCHECK_NULLPTR(Tasks[i], false)

		Tasks[i]->ResetPostCondition(Planner, PlanGenInfos);
	}

	return true;
}

bool USP_TaskChain::Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_BEGIN(Planner, TaskInfos)

	USP_TaskChainInfos* const Infos = Cast<USP_TaskChainInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_TaskChainInfos")

	SP_RCHECK(Tasks.Num(), false, "Empty tasks!")
	SP_RCHECK(Tasks[0], false, "Task[0] is nullptr!")

	Infos->TaskInfos = Tasks[0]->InstantiateInfos();

	return true;
}
ESP_PlanExecutionState USP_TaskChain::Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_TICK(DeltaSeconds, Planner, TaskInfos)

	USP_TaskChainInfos* const Infos = Cast<USP_TaskChainInfos>(TaskInfos);
	SP_RCHECK(Infos, ESP_PlanExecutionState::PES_Failed, "Infos nullptr! TaskInfos must be of type USP_TaskChainInfos")

	SP_RCHECK(Infos->Index < Tasks.Num(), ESP_PlanExecutionState::PES_Failed, "Index out of range!")
	SP_RCHECK(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed, "Task [%d] is nullptr!", Infos->Index)

	// Tick current task.
	ESP_PlanExecutionState TickResult = Tasks[Infos->Index]->Tick(DeltaSeconds, *Planner, Infos->TaskInfos);

	if (TickResult != ESP_PlanExecutionState::PES_Succeed)
		return TickResult; // Failed or rurnning.

	// Tick succeeded.

	// All task ended with success.
	if (++Infos->Index == Tasks.Num())
		return ESP_PlanExecutionState::PES_Succeed;

	// Begin next task.
	SP_RCHECK(Tasks[Infos->Index], ESP_PlanExecutionState::PES_Failed, "Task [%d] is nullptr!", Infos->Index)
	Infos->TaskInfos = Tasks[Infos->Index]->InstantiateInfos();

	return ESP_PlanExecutionState::PES_Running;
}
bool USP_TaskChain::End_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos)
{
	SP_TASK_SUPER_END(Planner, TaskInfos)

	USP_TaskChainInfos* const Infos = Cast<USP_TaskChainInfos>(TaskInfos);
	SP_RCHECK(Infos, false, "Infos nullptr! TaskInfos must be of type USP_TaskChainInfos")

	if (Infos->bForcedEnd)
	{
		SP_RCHECK_NULLPTR(Tasks[Infos->Index], false)
		Tasks[Infos->Index]->Cancel(*Planner, Infos->TaskInfos);
	}

	return true;
}