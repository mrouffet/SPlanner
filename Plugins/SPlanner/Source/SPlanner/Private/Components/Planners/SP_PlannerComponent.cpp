#include <Components/Planners/SP_PlannerComponent.h>

#include <TimerManager.h>

#include <Debug/SP_Debug.h>

#include <Miscs/SP_Settings.h>
#include <Miscs/SP_PlanConstructTask.h>

#include <Goal/SP_Goal.h>

#include <Actions/SP_ActionSet.h>
#include <Actions/SP_ActionStep.h>

#include <Actors/SP_Director.h>

#include <Components/SP_ActionSetComponent.h>

USP_PlannerComponent::USP_PlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickInEditor = false;
	PrimaryComponentTick.bCanEverTick = false;

	// Must be init in cpp file (Standard).
	PlanState = ESP_PlanState::PS_Invalid;
}

USP_Goal* USP_PlannerComponent::GetGoal() const
{
	return Goal;
}
void USP_PlannerComponent::SetGoal(USP_Goal* InGoal)
{
	if (Goal == InGoal)
		return;

	// Cancel previous plan.
	if (PlanState == ESP_PlanState::PS_Valid)
		OnPlanCancelled();

	// Out dated plan.
	if (PlanState != ESP_PlanState::PS_WaitForCompute)
	{
		GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);

		PlanState = ESP_PlanState::PS_Invalid;
	}

	USP_Goal* OldGoal = Goal;
	Goal = InGoal;

	OnGoalChange.Broadcast(this, OldGoal, Goal);

	// Do not ask again if already / still waiting for computation.
	if (PlanState != ESP_PlanState::PS_WaitForCompute)
		AskNewPlan();
}

void USP_PlannerComponent::SetNewPlan(TArray<USP_ActionStep*>&& InPlan)
{
#if SP_DEBUG
	if (PlanState == ESP_PlanState::PS_Valid)
		SP_LOG(Warning, "Set new plan while still valid!")

#if SP_DEBUG_EDITOR
		// Log plan.
		if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Plan) && GetOwner()->IsSelected())
		{
			FString PlanDebugStr = "Goal: " + Goal->GetName() + " --- Plan: ";

			if (InPlan.Num() != 0)
			{
				for (int i = 0; i < InPlan.Num(); i++)
					PlanDebugStr += InPlan[i]->GetName() + ", ";

				PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
			}
			else
				PlanDebugStr += "null";

			SP_LOG_SCREEN_FULL(Display, USP_Settings::GetPlanLogKey(), FColor::Orange, USP_Settings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
		}
#endif

#endif

	if (InPlan.Num() != 0)
	{
		Plan = std::move(InPlan);
		PlanState = ESP_PlanState::PS_Valid;
	}
	else
		PlanState = ESP_PlanState::PS_Invalid;
}

FSP_PlannerActionSet USP_PlannerComponent::CreatePlannerActionSet()
{
	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = ActionSet->GetActionSet(Goal);
	SP_RCHECK_NULLPTR(CurrActionSet, FSP_PlannerActionSet())

	return CurrActionSet->Shuffle();
}

void USP_PlannerComponent::AskNewPlan()
{
	SP_CHECK(PlanState != ESP_PlanState::PS_Valid, "Plan still valid!")
	SP_CHECK(PlanState != ESP_PlanState::PS_WaitForCompute, "Plan already waiting for being computed!")

#if SP_DEBUG_EDITOR
	// Reset debug keys.
	if (GetOwner()->IsSelectedInEditor())
		USP_Settings::ResetTaskExecuteLogKey();
#endif

	Plan.Empty();

	ESP_PlanState PrevState = PlanState;

	// Wait for new plan computation.
	PlanState = ESP_PlanState::PS_WaitForCompute;

	if (TimeBeforeConstructPlan <= 0.0f)
	{
		// Queue plan construction in thread.
		(new FAutoDeleteAsyncTask<FSP_PlanConstructTask>(this))->StartBackgroundTask();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ConstructPlanTimer,
			[this](){ (new FAutoDeleteAsyncTask<FSP_PlanConstructTask>(this))->StartBackgroundTask(); },
			TimeBeforeConstructPlan,
			false
		);
	}
}
void USP_PlannerComponent::ConstructPlan()
{
	SP_CHECK(PlanState == ESP_PlanState::PS_WaitForCompute, "PlanState must be waiting for computation at this point. This should never happen.")

	PlanState = ESP_PlanState::PS_Computing;

	const FSP_PlannerActionSet PlannerActions = CreatePlannerActionSet();

#if SP_DEBUG_EDITOR
	// Log available planner actions.
	if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Plan) && GetOwner()->IsSelected())
	{
		FString PlanDebugStr = "Action list: ";

		if (PlannerActions.BeginActions.Num() != 0)
		{
			PlanDebugStr += "\nBegin: ";

			for(int i = 0; i < PlannerActions.BeginActions.Num(); ++i)
				PlanDebugStr += PlannerActions.BeginActions[i].GetDebugStr() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}

		if (PlannerActions.ForcedActions.Num() != 0)
		{
			PlanDebugStr += "\nForced: ";

			for (int i = 0; i < PlannerActions.ForcedActions.Num(); ++i)
				PlanDebugStr += PlannerActions.ForcedActions[i].GetDebugStr() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}

		if (PlannerActions.Actions.Num() != 0)
		{
			if(PlanDebugStr != "Move list: ")
				PlanDebugStr += "\nCore: ";

			for (int i = 0; i < PlannerActions.Actions.Num(); ++i)
				PlanDebugStr += PlannerActions.Actions[i].GetDebugStr() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}
		else if (PlanDebugStr == "Move list: ")
			PlanDebugStr += "null";

		SP_LOG_SCREEN_FULL(Display, USP_Settings::GetActionListLogKey(), FColor::Yellow, USP_Settings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
	}
#endif

	TArray<USP_ActionStep*> NewPlan;

	// Planner Computation.
	if (ConstructPlan_Internal(PlannerActions, NewPlan))
	{
		// Plan still being computed by this thread (not cancelled with AskNewPlan or SetNewGoal on main thread).
		if(PlanState == ESP_PlanState::PS_Computing)
			SetNewPlan(std::move(NewPlan));
	}
	else
	{
		PlanState = ESP_PlanState::PS_Invalid;
		
		// No plan found.
		OnPlanConstructionFailed();
	}
}
bool USP_PlannerComponent::ConstructPlan_Internal(const FSP_PlannerActionSet& PlannerActions, TArray<USP_ActionStep*>& OutPlan, FSP_PlannerFlags PlannerFlags, uint8 CurrDepth) const
{
	if (CurrDepth > MaxPlannerDepth)
		return false;

	int Index = CurrDepth;
	const TArray<FSP_PlannerAction>* Actions = &PlannerActions.Actions;

	// Use BeginActions if any.
	if (PlannerActions.BeginActions.Num())
	{
		if (CurrDepth == 0)
			Actions = &PlannerActions.BeginActions;
		else
			--Index; // Remove Begin depth incrementation.
	}

	// TODO: Use Forced Actions.

	for (int i = Index; i < Actions->Num(); ++i)
	{
		const FSP_PlannerAction& Action = (*Actions)[i];
		SP_CCHECK_NULLPTR(Action.Step)

		if (!Action.Step->PreCondition(this, PlannerFlags))
			continue;

		// Add steps to current plan.
		OutPlan.Add(Action.Step);

		// Check if action achieve plan or plan with this task achieve plan.
		if (Action.bAchievePlan || ConstructPlan_Internal(PlannerActions, OutPlan, Action.Step->PostCondition(this, PlannerFlags), CurrDepth + 1))
			return true;

		// Plan generation failed, remove this task from plan.
		OutPlan.RemoveAt(static_cast<int32>(Index)); // Remove last.
	}

	return false;
}

void USP_PlannerComponent::OnPlanConstructionFailed_Implementation()
{
}
void USP_PlannerComponent::OnPlanCancelled_Implementation()
{
	SP_CHECK(PlanState == ESP_PlanState::PS_Valid, "Try to cancel invalid plan!")
}

void USP_PlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Computed by server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	// Ask plan with default goal.
	if (Goal)
		AskNewPlan();

	// Register in Director.
	if (bAutoRegisterInDirector)
		ASP_Director::Register(this);
}
void USP_PlannerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Computed by server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	// Unregister from Director.
	if (bAutoRegisterInDirector)
		ASP_Director::TryUnRegister(this); // Try unregister: Director can be destroyed first during scene travel or quit game.
}