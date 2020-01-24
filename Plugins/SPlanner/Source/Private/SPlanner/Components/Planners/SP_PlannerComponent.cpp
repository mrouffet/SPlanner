#include <SPlanner/Components/Planners/SP_PlannerComponent.h>

#include <TimerManager.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Miscs/SP_Settings.h>
#include <SPlanner/Miscs/SP_PlanConstructTask.h>

#include <SPlanner/Goal/SP_Goal.h>

#include <SPlanner/Actions/SP_ActionSet.h>
#include <SPlanner/Actions/SP_ActionStep.h>

#include <SPlanner/Actors/SP_Director.h>

#include <SPlanner/Components/SP_ActionSetComponent.h>
#include <SPlanner/Components/Zones/SP_PlannerLODComponent.h>

USP_PlannerComponent::USP_PlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickInEditor = false;
	PrimaryComponentTick.bCanEverTick = true;

	// Must be init in cpp file (Standard).
	PlanState = ESP_PlanState::PS_Inactive;
}

ESP_PlanState USP_PlannerComponent::GetPlanState() const
{
	return PlanState;
}

void USP_PlannerComponent::SetEnableBehavior(bool bEnable)
{
	if (bEnable)
		OnActive_Internal();
	else
		OnInactive_Internal();
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
	// Check current goal can transit to (SetGoal can be called multiple time in one frame).
	if(bForce || !Goal || Goal->CanTransitTo(InGoal))
		NextGoal = InGoal;
}
void USP_PlannerComponent::UpdateGoal()
{
	if (Goal == NextGoal)
		return;

	USP_Goal* OldGoal = Goal;
	Goal = NextGoal;

	OnGoalChange.Broadcast(this, OldGoal, Goal);

	// Inactive planner.
	if (PlanState == ESP_PlanState::PS_Inactive)
		return;
	else if (PlanState == ESP_PlanState::PS_Valid) // Cancel previous plan.
	{
		CancelPlan();

		PlanState = ESP_PlanState::PS_Finished;
	}

	// Out dated plan: Do not ask again if already / still waiting for computation.
	if (PlanState != ESP_PlanState::PS_WaitForCompute)
	{
		// ConstructPlanTimer may be used for waiting other instructions (see SP_AIPlannerComponent).
		GetWorld()->GetTimerManager().ClearTimer(ConstructPlanTimer);

		if (!Goal)
			PlanState = ESP_PlanState::PS_Invalid;
		else
			AskNewPlan(true); // Instant request new plan with goal.
	}
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

FSP_PlannerActionSet USP_PlannerComponent::CreatePlannerActionSet(float LODLevel) const
{
	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = ActionSet->GetActionSet(Goal);
	SP_RCHECK_NULLPTR(CurrActionSet, FSP_PlannerActionSet())

	return CurrActionSet->Shuffle(LODLevel);
}

void USP_PlannerComponent::AskNewPlan(bool bInstantRequest)
{
	SP_CHECK(PlanState != ESP_PlanState::PS_Valid, "Plan still valid!")
	SP_CHECK(PlanState != ESP_PlanState::PS_WaitForCompute, "Plan already waiting for being computed!")
	SP_CHECK(PlanState != ESP_PlanState::PS_Inactive, "Plan LOD is inactive!")

#if SP_DEBUG_EDITOR
	// Reset debug keys.
	if (GetOwner()->IsSelectedInEditor())
		USP_Settings::ResetTaskExecuteLogKey();
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
	// Log available planner actions.
	if (SP_IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Plan) && GetOwner()->IsSelected())
		SP_LOG_SCREEN_FULL(Display, USP_Settings::GetPlanGenerationLogKey(), FColor::Purple, USP_Settings::GetDebugScreenDisplayTime(), "Time before construct: %f", TimeBeforeConstructPlan)
#endif

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

	const FSP_PlannerActionSet PlannerActions = CreatePlannerActionSet(LODLevel);

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
			if(PlanDebugStr != "Action list: ")
				PlanDebugStr += "\nCore: ";

			for (int i = 0; i < PlannerActions.Actions.Num(); ++i)
				PlanDebugStr += PlannerActions.Actions[i].GetDebugStr() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}
		else if (PlanDebugStr == "Action list: ")
			PlanDebugStr += "null";

		SP_LOG_SCREEN_FULL(Display, USP_Settings::GetActionListLogKey(), FColor::Yellow, USP_Settings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
	}
#endif

	TArray<USP_ActionStep*> NewPlan;

	int8 MaxDepth = LODLevel > 0.0f ? LOD->GetMaxPlannerDepth(LODLevel) : DefaultMaxPlannerDepth;

#if SP_DEBUG
	if (MaxDepth <= 0)
	{
		SP_LOG(Error, "Bad MaxDepth: %d", MaxDepth)
		OnPlanConstructionFailed(ESP_PlanError::PE_BadArgument);
		return;
	}
#endif

	// Planner Computation.
	if (ConstructPlan_Internal(PlannerActions, NewPlan, MaxDepth))
	{
		// Plan still being computed by this thread (not cancelled with AskNewPlan or SetNewGoal on main thread).
		if(PlanState == ESP_PlanState::PS_Computing)
			SetNewPlan(std::move(NewPlan));
	}
	else // No plan found.
		OnPlanConstructionFailed(ESP_PlanError::PE_ConstructionFailed);
}
bool USP_PlannerComponent::ConstructPlan_Internal(const FSP_PlannerActionSet& PlannerActions,
	TArray<USP_ActionStep*>& OutPlan,
	uint8 MaxDepth,
	uint8 CurrDepth,
	uint64 PlannerFlags) const
{
	if (CurrDepth > MaxDepth)
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

#if SP_DEBUG
	// TODO: Use Forced Actions.
	if(PlannerActions.ForcedActions.Num())
		SP_LOG(Warning, "Forced actions not implemented yet! ActionSet: %s", *ActionSet->GetActionSet(Goal)->GetName())
#endif

	for (int i = Index; i < Actions->Num(); ++i)
	{
		const FSP_PlannerAction& Action = (*Actions)[i];
		SP_CCHECK_NULLPTR(Action.Step)

		if (!Action.Step->PreCondition(this, PlannerFlags))
			continue;

		// Add steps to current plan.
		OutPlan.Add(Action.Step);

		// Check if action achieve plan or plan with this task achieve plan.
		if (Action.bAchievePlan || ConstructPlan_Internal(PlannerActions, OutPlan, MaxDepth, CurrDepth + 1, Action.Step->PostCondition(this, PlannerFlags)))
			return true;

		// Plan generation failed, remove this task from plan.
		OutPlan.RemoveAt(static_cast<int32>(Index)); // Remove last.
	}

	return false;
}

void USP_PlannerComponent::OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError)
{
	PlanState = ESP_PlanState::PS_Invalid;
}

bool USP_PlannerComponent::OnActive_Internal_Implementation()
{
	if (PlanState != ESP_PlanState::PS_Inactive)
		return false;

	// Set as finished to ask a new plan.
	PlanState = ESP_PlanState::PS_Finished;

	if (Goal) // Active goal already set.
		AskNewPlan();

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

	if(bResetGoalOnInactive)
		SetGoal(nullptr);

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

void USP_PlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Computed by server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	NextGoal = Goal;

	if (bStartActive)
	{
		// Register before set active.
		if (bAutoRegisterInDirector)
			ASP_Director::Register(this);

		OnActive_Internal();
	}
	else
	{
		// Set previouly as inactive to success OnInactive().
		PlanState = ESP_PlanState::PS_Finished;

		OnInactive_Internal();

		// Register after inactive (avoid double call).
		if (bAutoRegisterInDirector)
			ASP_Director::Register(this);
	}
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
void USP_PlannerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update new goal.
	UpdateGoal();
}