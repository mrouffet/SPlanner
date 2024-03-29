// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <TimerManager.h>

#include <SPlanner/SP_Config.h>
#include <SPlanner/Misc/SP_FlagHelper.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Base/Action/SP_Action.h>
#include <SPlanner/Base/Action/SP_ActionImpl.h>
#include <SPlanner/Base/Action/SP_PlannerActionSet.h>

#include <SPlanner/Base/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/Base/Director/SP_Director.h>

#if SP_DEBUG_EDITOR

#include <SPlannerEditor/Misc/SP_EditorSettings.h>

#endif

USP_PlannerComponent::USP_PlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickInEditor = false;
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	/**
	*	Must be init in cpp file (Standard).
	*	Strat as invalid (not initialized).
	*	Will be set to Valid / Inactive in BeginPlay.
	*/
	PlanState = ESP_PlanState::PS_Invalid;
}

ESP_PlanState USP_PlannerComponent::GetPlanState() const
{
	return PlanState;
}

bool USP_PlannerComponent::IsBehaviorEnabled() const
{
	return PlanState != ESP_PlanState::PS_Inactive;
}
void USP_PlannerComponent::SetEnableBehavior_Implementation(bool bEnable)
{
	if (IsBehaviorEnabled() == bEnable)
		return;

	if (bEnable)
		OnActive_Internal();
	else
		OnInactive_Internal();
}

USP_Goal* USP_PlannerComponent::GetGoal() const
{
	return Goal;
}
void USP_PlannerComponent::SetGoal(USP_Goal* InGoal, bool bForce)
{
	// Same goal or current goal can transit to.
	if (Goal == InGoal || (!bForce && Goal && !USP_Goal::CanTransitTo(this, Goal, InGoal)))
		return;

	if (PlanState == ESP_PlanState::PS_Valid) // Cancel previous plan.
	{
		CancelPlan();

		PlanState = ESP_PlanState::PS_Finished;
	}

	// Set new goal after plan state change and possible cancel.
	USP_Goal* OldGoal = Goal;
	Goal = InGoal;

	// Inactive planner.
	if (PlanState == ESP_PlanState::PS_Inactive)
		return;

	if (OldGoal)
		OldGoal->OnEnd(this);
	if (Goal)
	{
		Goal->OnStart(this);

		// Out dated plan: Do not ask again if already / still waiting for computation.
		if (PlanState != ESP_PlanState::PS_WaitForCompute)
		{
			// ConstructPlanTimer may be used for waiting other instructions (see SP_AIPlannerComponent).
			GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);

			AskNewPlan();
		}
	}
	else
	{
		// New goal nullptr: set plan to invalid and clear any construct timer.

		PlanState = ESP_PlanState::PS_Invalid;
		GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);
	}

	OnGoalChange.Broadcast(this, OldGoal, Goal);
}

const TArray<USP_ActionImpl*>& USP_PlannerComponent::GetPlan() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Plan, "Invalid plan: performing unsafe operation!")

	return Plan;
}

bool USP_PlannerComponent::CancelPlan_Implementation()
{
	// Must cancel a valid plan.
	if (PlanState != ESP_PlanState::PS_Valid)
		return false;

	OnPlanCancel.Broadcast(this);

	return true;
}

void USP_PlannerComponent::SetNewPlan(TArray<USP_ActionImpl*>&& InPlan)
{
#if SP_DEBUG
	if (PlanState == ESP_PlanState::PS_Valid)
		SP_LOG(Warning, "Set new plan while still valid!")

#if SP_DEBUG_EDITOR || SP_LOG_PLAN
	// Log plan.
	FString PlanDebugStr = "Goal: " + Goal->GetName() + " --- Plan: ";

	if (InPlan.Num() != 0)
	{
		for (int i = 0; i < InPlan.Num(); i++)
			PlanDebugStr += InPlan[i]->GetName() + ", ";

		PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
	}
	else
		PlanDebugStr += "null";

#if SP_DEBUG_EDITOR
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Plan) && IsSelectedInEditor())
		SP_LOG_SCREEN_FULL(Display, USP_EditorSettings::GetPlanLogKey(), FColor::Orange, USP_EditorSettings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
#endif

#if SP_LOG_PLAN
	UE_LOG(LogSP_Debug, Display, "%s: %s", *GetOwner()->GetName(), *PlanDebugStr)
#endif

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

FSP_PlannerActionSet USP_PlannerComponent::CreatePlannerActionSet(bool* bCanBeAchievedPtr) const
{
	SP_BENCHMARK_SCOPE(PC_CreatePlannerActionSet)

	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(Blackboard, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = Blackboard->GetActionSet(Goal);
	SP_RCHECK_NULLPTR(CurrActionSet, FSP_PlannerActionSet())

	return CurrActionSet->Shuffle(this, bCanBeAchievedPtr);
}

void USP_PlannerComponent::AskNewPlan(bool bInstantRequest)
{
	SP_BENCHMARK_SCOPE(PC_AskNewPlan)

	SP_CHECK(PlanState != ESP_PlanState::PS_Valid, "Plan still valid!")
	SP_CHECK(PlanState != ESP_PlanState::PS_WaitForCompute, "Plan already waiting for being computed!")
	SP_CHECK(PlanState != ESP_PlanState::PS_Inactive, "Plan LOD is inactive!")

#if SP_DEBUG_EDITOR
	// Reset debug keys.
	if (GetOwner()->IsSelectedInEditor())
		USP_EditorSettings::ResetTaskExecuteLogKey();
#endif

	Plan.Empty();

	// Wait for new plan computation.
	PlanState = ESP_PlanState::PS_WaitForCompute;

	OnAskPlan.Broadcast(this);

	float TimeBeforeConstructPlan = bInstantRequest ? -1.0f : QueryTimeBeforeConstructPlan();

#if SP_DEBUG_EDITOR
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Plan) && IsSelectedInEditor())
		SP_LOG_SCREEN_FULL(Display, USP_EditorSettings::GetPlanGenerationLogKey(), FColor::Purple, USP_EditorSettings::GetDebugScreenDisplayTime(), "Time before construct: %f", TimeBeforeConstructPlan)
#endif

	if (TimeBeforeConstructPlan <= 0.0f)
		ConstructPlan();
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ConstructPlanTimer,
			[this]() { ConstructPlan(); },
			TimeBeforeConstructPlan,
			false
		);
	}
}
void USP_PlannerComponent::ConstructPlan()
{
	SP_BENCHMARK_SCOPE(PC_ConstructPlan)

	// LOD became out of range during TimeBeforeConstructPlan.
	if (PlanState == ESP_PlanState::PS_Inactive)
		return;

	SP_CHECK(PlanState == ESP_PlanState::PS_WaitForCompute, "PlanState must be waiting for computation at this point. This should never happen.")

	PlanState = ESP_PlanState::PS_Computing;

	USP_PlanGenInfos* Infos = NewObject<USP_PlanGenInfos>(this, PlanGenInfosClass);
	Infos->Planner = this;

	bool bCanBeAchieved = false;
	Infos->PlannerActions = CreatePlannerActionSet(&bCanBeAchieved);

	// Check plan valid for constrution.
	if (!bCanBeAchieved)
	{
		OnPlanConstructionFailed(ESP_PlanError::PE_CantBeAchieved);
		return;
	}

#if SP_DEBUG_EDITOR
	// Log available planner actions.
	if (SP_IS_FLAG_SET(USP_EditorSettings::GetDebugMask(), ESP_EditorDebugFlag::ED_Plan) && IsSelectedInEditor())
	{
		FString PlanDebugStr = "Action list: ";

		if (Infos->PlannerActions.BeginActions.Num() != 0)
		{
			PlanDebugStr += "\nBegin: ";

			for(int i = 0; i < Infos->PlannerActions.BeginActions.Num(); ++i)
				PlanDebugStr += Infos->PlannerActions.BeginActions[i].Handle->GetDebugStr() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}

		if (Infos->PlannerActions.Actions.Num() != 0)
		{
			if(PlanDebugStr != "Action list: ")
				PlanDebugStr += "\nCore: ";

			for (int i = 0; i < Infos->PlannerActions.Actions.Num(); ++i)
			{
				if (Infos->PlannerActions.Actions[i].bIsForced)
					PlanDebugStr += "F_"; // Forced action.
				else if (Infos->PlannerActions.Actions[i].bAchievePlan)
					PlanDebugStr += "E_"; // End action.
				else
					PlanDebugStr += "C_"; // Core action.

				PlanDebugStr += Infos->PlannerActions.Actions[i].Handle->GetDebugStr() + ", ";
			}

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}
		else if (PlanDebugStr == "Action list: ")
			PlanDebugStr += "null";

		SP_LOG_SCREEN_FULL(Display, USP_EditorSettings::GetActionListLogKey(), FColor::Yellow, USP_EditorSettings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
	}
#endif

	Infos->SetMaxDepth(QueryMaxPlannerDepth());

#if SP_DEBUG
	if (Infos->MaxDepth <= 0)
	{
		SP_LOG(Error, "Bad MaxDepth: %d", Infos->MaxDepth)
		OnPlanConstructionFailed(ESP_PlanError::PE_BadArgument);
		return;
	}
#endif

	// Planner Computation.
	if (ConstructPlan_Internal(Infos))
	{
		// Plan still being computed by this thread (not cancelled with AskNewPlan or SetNewGoal on main thread).
		if(PlanState == ESP_PlanState::PS_Computing)
			SetNewPlan(std::move(Infos->OutPlan));
	}
	else // No plan found.
		OnPlanConstructionFailed(ESP_PlanError::PE_CantBeAchieved);
}
bool USP_PlannerComponent::ConstructPlan_Internal(USP_PlanGenInfos* Infos) const
{
	SP_LOG(Error, "Implementation must be overridden in children.")

	return false;
}

void USP_PlannerComponent::OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError)
{
#if SP_WARNING_PLAN_GEN_FAILED
	SP_LOG(Warning, "Plan Generation Failed: %s", *FindObject<UEnum>(ANY_PACKAGE, TEXT("ESP_PlanError"), true)->GetNameStringByIndex(static_cast<int32>(PlanError)))
#endif

	PlanState = ESP_PlanState::PS_Invalid;
}

bool USP_PlannerComponent::OnActive_Internal_Implementation()
{
	if (PlanState != ESP_PlanState::PS_Inactive)
		return false;

	// Set as finished to ask a new plan.
	PlanState = ESP_PlanState::PS_Finished;

	if (Goal) // Active goal already set.
	{
		Goal->OnStart(this);
		AskNewPlan();
	}

	OnActive.Broadcast(this);

	return true;
}
bool USP_PlannerComponent::OnInactive_Internal_Implementation()
{
	if (PlanState == ESP_PlanState::PS_Inactive)
		return false;

	CancelPlan();
	GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);

	PlanState = ESP_PlanState::PS_Inactive;

	// Blackboard can be null while planner is waiting for possess.
	if (Blackboard)
	{
		// End goal manually because PlanState is set to Inactive.
		if (Goal)
			Goal->OnEnd(this);
		if (bResetGoalOnInactive)
			SetGoal(nullptr, true);
	}
	// Do not reset goal while blackboard is null (waiting for possess).

	OnInactive.Broadcast(this);

	return true;
}

float USP_PlannerComponent::QueryTimeBeforeConstructPlan_Implementation() const
{
	return DefaultTimeBeforeConstructPlan;
}
int USP_PlannerComponent::QueryMaxPlannerDepth_Implementation() const
{
	return DefaultMaxPlannerDepth;
}

void USP_PlannerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Computed by server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	if (bStartActive && Blackboard) // Wait for possessed blackboard.
	{
		// PlanState start as Invalid. Set Inactive to Activate.
		PlanState = ESP_PlanState::PS_Inactive;

		// Register before set active.
		if (bAutoRegisterInDirector)
			ASP_Director::RegisterPlanner(this);

		OnActive_Internal();
	}
	else
	{
		// Set previouly as inactive to success OnInactive().
		PlanState = ESP_PlanState::PS_Finished;

		OnInactive_Internal();

		// Register after inactive (avoid double call).
		if (bAutoRegisterInDirector)
			ASP_Director::RegisterPlanner(this);
	}
}
void USP_PlannerComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	// Computed by server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	// Unregister from Director. Check valid instance: Director could have been destroyed first during scene travel or quit game.
	if (bAutoRegisterInDirector && ASP_Director::GetInstance())
		ASP_Director::UnRegisterPlanner(this);
}

void USP_PlannerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure timer clean.
	GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);
}

#if WITH_EDITOR

bool USP_PlannerComponent::IsSelectedInEditor() const
{
	// This component selected.
	if (Super::IsSelectedInEditor())
		return true;

	// Owner Actor selected.
	if (GetOwner())
		return GetOwner()->IsSelectedInEditor();

	return false;
}

#endif