// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <TimerManager.h>

#include <SPlanner/SP_Config.h>
#include <SPlanner/Miscs/SP_FlagHelper.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Base/Actions/SP_ActionStep.h>
#include <SPlanner/Base/Actions/SP_PlannerActionSet.h>

#include <SPlanner/Base/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/Base/Director/SP_Director.h>

#include <SPlanner/Base/Zones/SP_PlannerLODComponent.h>

#if SP_DEBUG_EDITOR

#include <SPlannerEditor/Miscs/SP_EditorSettings.h>

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
void USP_PlannerComponent::SetEnableBehavior(bool bEnable)
{
	if (bEnable)
		OnActive_Internal();
	else
		OnInactive_Internal();
}

float USP_PlannerComponent::GetLODLevel() const
{
	if (LOD && LOD->IsInRange())
		return LOD->GetLevel();

	return -1.0f;
}
void USP_PlannerComponent::SetLOD(USP_PlannerLODComponent* NewLOD)
{
	if (LOD)
	{
		LOD->OnActive.RemoveDynamic(this, &USP_PlannerComponent::OnActiveLOD);
		LOD->OnInactive.RemoveDynamic(this, &USP_PlannerComponent::OnInactiveLOD);
	}

	LOD = NewLOD;

	// Can be nullptr to reset component.
	if (LOD)
	{
		LOD->OnActive.AddDynamic(this, &USP_PlannerComponent::OnActiveLOD);
		LOD->OnInactive.AddDynamic(this, &USP_PlannerComponent::OnInactiveLOD);
	}
}

USP_Goal* USP_PlannerComponent::GetGoal() const
{
	return Goal;
}
void USP_PlannerComponent::SetGoal(USP_Goal* InGoal, bool bForce)
{
	// Same goal or current goal can transit to.
	if (Goal == InGoal || (Goal && !USP_Goal::CanTransitTo(this, Goal, InGoal, bForce)))
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
		// Blackboard must be reset to start this goal.
		if (Goal->GetResetBlackboard())
		{
			SP_CHECK_NULLPTR(Blackboard)
			Blackboard->Reset();
		}

		// Start after reset.
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

const TArray<USP_ActionStep*>& USP_PlannerComponent::GetPlan() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Plan, "Invalid plan: performing unsafe operation!")

	return Plan;
}

bool USP_PlannerComponent::CancelPlan()
{
	// Must cancel a valid plan.
	if (PlanState != ESP_PlanState::PS_Valid)
		return false;

	OnPlanCancel.Broadcast(this);

	return true;
}

void USP_PlannerComponent::SetNewPlan(TArray<USP_ActionStep*>&& InPlan)
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

FSP_PlannerActionSet USP_PlannerComponent::CreatePlannerActionSet(float LODLevel, bool* bCanBeAchievedPtr) const
{
	SP_BENCHMARK_SCOPE(PC_CreatePlannerActionSet)

	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(Blackboard, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = Blackboard->GetActionSet(Goal);
	SP_RCHECK_NULLPTR(CurrActionSet, FSP_PlannerActionSet())

	return CurrActionSet->Shuffle(LODLevel, bCanBeAchievedPtr);
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

	float TimeBeforeConstructPlan = DefaultTimeBeforeConstructPlan;

	if (bInstantRequest)
		TimeBeforeConstructPlan = -1.0f;
	else if (LOD)
	{
		SP_CHECK(LOD->IsInRange(), "LOD inactive!")
		TimeBeforeConstructPlan = LOD->GetTimeBeforeConstructPlan();
	}

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

	// Get current LOD level.
	float LODLevel = -1.0f;

	if (LOD)
	{
#if SP_DEBUG
		if (!LOD->IsInRange())
		{
			SP_LOG(Error, "LOD inactive!")
			OnPlanConstructionFailed(ESP_PlanError::PE_LODOutOfRange);
			return;
		}
#endif
		LODLevel = LOD->GetLevel();
	}

	bool bCanBeAchieved = false;
	FSP_PlannerActionSet PlannerActions = CreatePlannerActionSet(LODLevel, &bCanBeAchieved);

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

		if (PlannerActions.BeginActions.Num() != 0)
		{
			PlanDebugStr += "\nBegin: ";

			for(int i = 0; i < PlannerActions.BeginActions.Num(); ++i)
				PlanDebugStr += PlannerActions.BeginActions[i].Handle->GetDebugStr() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}

		if (PlannerActions.Actions.Num() != 0)
		{
			if(PlanDebugStr != "Action list: ")
				PlanDebugStr += "\nCore: ";

			for (int i = 0; i < PlannerActions.Actions.Num(); ++i)
			{
				if (PlannerActions.Actions[i].bIsForced)
					PlanDebugStr += "F_"; // Forced action.
				else if (PlannerActions.Actions[i].bAchievePlan)
					PlanDebugStr += "E_"; // End action.
				else
					PlanDebugStr += "C_"; // Core action.

				PlanDebugStr += PlannerActions.Actions[i].Handle->GetDebugStr() + ", ";
			}

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}
		else if (PlanDebugStr == "Action list: ")
			PlanDebugStr += "null";

		SP_LOG_SCREEN_FULL(Display, USP_EditorSettings::GetActionListLogKey(), FColor::Yellow, USP_EditorSettings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
	}
#endif

	int8 MaxDepth = LODLevel > 0.0f ? LOD->GetMaxPlannerDepth(LODLevel) : DefaultMaxPlannerDepth;

#if SP_DEBUG
	if (MaxDepth <= 0)
	{
		SP_LOG(Error, "Bad MaxDepth: %d", MaxDepth)
		OnPlanConstructionFailed(ESP_PlanError::PE_BadArgument);
		return;
	}
#endif

	// Output plan.
	TArray<USP_ActionStep*> NewPlan;
	NewPlan.Reserve(MaxDepth);

	// Planner Computation.
	if (ConstructPlan_Internal(PlannerActions, NewPlan, NewObject<USP_PlanGenInfos>(this, PlanGenInfosClass), MaxDepth, LODLevel))
	{
		// Plan still being computed by this thread (not cancelled with AskNewPlan or SetNewGoal on main thread).
		if(PlanState == ESP_PlanState::PS_Computing)
			SetNewPlan(std::move(NewPlan));
	}
	else // No plan found.
		OnPlanConstructionFailed(ESP_PlanError::PE_CantBeAchieved);
}
bool USP_PlannerComponent::ConstructPlan_Internal(FSP_PlannerActionSet& PlannerActions,
	TArray<USP_ActionStep*>& OutPlan,
	USP_PlanGenInfos* PlanGenInfos,
	uint8 MaxDepth,
	float LODLevel) const
{
	// Must be overridden in children.

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

	// Try cancel plan, otherwise stop cosntruct timer.
	if(!CancelPlan())
		GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);

	PlanState = ESP_PlanState::PS_Inactive;

	// Blackboard can be null while planner is waiting for possess.
	if (Blackboard)
	{
		Blackboard->Reset();

		// End goal manually because PlanState is set to Inactive.
		if (Goal)
			Goal->OnEnd(this);
		if (bResetGoalOnInactive)
			SetGoal(nullptr);
	}
	// Do not reset goal while blackboard is null (waiting for possess).

	OnInactive.Broadcast(this);

	return true;
}

void USP_PlannerComponent::OnActiveLOD()
{
	SetEnableBehavior(true);
}
void USP_PlannerComponent::OnInactiveLOD()
{
	SetEnableBehavior(false);
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