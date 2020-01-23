#include <SPlanner/Components/Planners/SP_AIPlannerComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Miscs/SP_Settings.h>
#include <SPlanner/Miscs/SP_PlanState.h>

#include <SPlanner/Goal/SP_Goal.h>

#include <SPlanner/Actions/SP_ActionSet.h>
#include <SPlanner/Actions/SP_POIActionSet.h>
#include <SPlanner/Actions/AITasks/SP_AITask.h>

#include <SPlanner/Components/SP_POIComponent.h>
#include <SPlanner/Components/SP_TargetComponent.h>
#include <SPlanner/Components/SP_ActionSetComponent.h>
#include <SPlanner/Components/Zones/SP_POIZoneComponent.h>

USP_AIPlannerComponent::USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Execute task in tick or check cooldowns.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}

float USP_AIPlannerComponent::GetCooldown(const USP_AITask* Task) const
{
	SP_RCHECK_NULLPTR(Task, -1.0f)

	const float* const CooldownPtr = Cooldowns.Find(Task);

	return CooldownPtr ? GetWorld()->GetTimeSeconds() - *CooldownPtr : FLT_MAX;
}
void USP_AIPlannerComponent::SetCooldown(const USP_AITask* Task)
{
	SP_CHECK_NULLPTR(Task)

	// Never save task without cooldown.
	if(Task->GetCooldown() <= 0.0f)
		return;

	float* const CooldownPtr = Cooldowns.Find(Task);

	if (CooldownPtr)
		*CooldownPtr = GetWorld()->GetTimeSeconds() + Task->GetCooldown();
	else
		Cooldowns.Add(Task, GetWorld()->GetTimeSeconds() + Task->GetCooldown());
}
bool USP_AIPlannerComponent::IsInCooldown(const USP_AITask* Task) const
{
	SP_RCHECK_NULLPTR(Task, false)

	// Never check task without cooldown.
	if(Task->GetCooldown() <= 0.0f)
		return false;

	return GetCooldown(Task) < Task->GetCooldown();
}

void USP_AIPlannerComponent::OnGoalChange_Bind(USP_PlannerComponent* Planner, USP_Goal* OldGoal, USP_Goal* NewGoal)
{
	if (bResetTargetOnGoalChange && Target)
		Target->Clear();
}

void USP_AIPlannerComponent::CheckCooldowns()
{
	// Called on planState == EPlanState::PS_WaitForCooldowns.

	for (auto it = Cooldowns.begin(); it != Cooldowns.end(); ++it)
	{
		//Check newly available task (0.5 sec).
		if (FMath::Abs(GetWorld()->GetTimeSeconds() - it->Value - it->Key->GetCooldown()) <= 0.5f)
		{
			AskNewPlan();
			return;
		}
	}

	/*
	*	No newly available task which serves the goal.
	*	Plan still get invalid.
	*/
}

bool USP_AIPlannerComponent::BeginNextTask()
{
	// index == Plan.Num() will be checked to ask new plan.
	SP_RCHECK(CurrentPlanIndex + 1 >= 0 && CurrentPlanIndex + 1 <= Plan.Num(), "Index [%d] out of range [0, %d]!", false, (CurrentPlanIndex + 1), Plan.Num())

	++CurrentPlanIndex;
	
#if SP_DEBUG_EDITOR
	// Update debug keys.
	if(GetOwner()->IsSelectedInEditor())
		USP_Settings::IncrementTaskExecuteLogKey();
#endif

	// No other task left.
	if (CurrentPlanIndex == Plan.Num())
	{
		PlanState = ESP_PlanState::PS_Finished;

		AskNewPlan();
		return false;
	}

	USP_AITask* CurrentTask = Cast<USP_AITask>(Plan[CurrentPlanIndex]);
	SP_RCHECK_NULLPTR(CurrentTask, false)

	// Reserve user data.
	if (uint32 UserDataSize = CurrentTask->GetUserDataSize())
		TaskUserData.Reserve(UserDataSize);

	// Can't begin task, Plan got invalid: ask a new one.
	if (!CurrentTask->Begin(this, TaskUserData.GetData()))
	{
		if (CurrentTask->GetUseCooldownOnFailed())
			SetCooldown(CurrentTask);
			
		PlanState = ESP_PlanState::PS_Invalid;

		AskNewPlan();

		return false;
	}

	return true;
}
void USP_AIPlannerComponent::ExecuteTask(float DeltaTime)
{
	// Execute first task begin on main thread.
	if (CurrentPlanIndex == -1)
	{
		// Try to begin first task.
		if (!BeginNextTask())
			return;
	}

	SP_CHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())
	USP_AITask* CurrentTask = Cast<USP_AITask>(Plan[CurrentPlanIndex]);
	SP_CHECK_NULLPTR(CurrentTask)

	ESP_PlanExecutionState TickResult = CurrentTask->Tick(DeltaTime, this, TaskUserData.GetData());

	// Process task result.
	if (TickResult == ESP_PlanExecutionState::PES_Running)
		return;

	// Always end task (Tick succeed or failed).
	if (!CurrentTask->End(this, TaskUserData.GetData()) || TickResult == ESP_PlanExecutionState::PES_Failed)
	{
		if (CurrentTask->GetUseCooldownOnFailed())
			SetCooldown(CurrentTask);

		// Plan got invalid: ask a new one.
		PlanState = ESP_PlanState::PS_Invalid;

		AskNewPlan();
	}
	else
	{
		// Task succeed.
		SetCooldown(CurrentTask);

		BeginNextTask();
	}
}

FSP_PlannerActionSet USP_AIPlannerComponent::CreatePlannerActionSet(float LODLevel) const
{
	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = ActionSet->GetActionSet(Goal);
	SP_RCHECK_NULLPTR(CurrActionSet, FSP_PlannerActionSet())

	struct CooldownPredicate
	{
		USP_AIPlannerComponent* Planner = nullptr;

		bool operator()(const FSP_Action& Action) const
		{
			return !Planner->IsInCooldown(Cast<USP_AITask>(Action.Step));
		}
	};

	FSP_PlannerActionSet PlannerActions = CurrActionSet->Shuffle(LODLevel, CooldownPredicate());

	// Add all available actions from POI.
	if (POIZone)
	{
		for (int j = 0; j < POIZone->GetPOIs().Num(); ++j)
		{
			SP_CCHECK(POIZone->GetPOIs()[j], "%s: POI [ %d ] nullptr!", false, *GetName(), j)
			SP_CCHECK(POIZone->GetPOIs()[j]->GetActionSet(), "%s: POI [ %s ] action set nullptr!", false, *GetName(), *POIZone->GetPOIs()[j]->GetOwner()->GetName())

			const TArray<FSP_POIAction>& POIActions = POIZone->GetPOIs()[j]->GetActionSet()->GetActions();

			for (int i = 0; i < POIActions.Num(); ++i)
			{
				SP_CCHECK(POIActions[i].GetTask(), "%s: POI Task [ %d ] nullptr!", *POIZone->GetPOIs()[j]->GetActionSet()->GetName(), i)

				// Use INDEX_NONE to convert int32 to bool.
				bool bAchieveGoal = POIActions[i].GetAchievedGoals().Find(Goal) != INDEX_NONE;

				if (!IsInCooldown(POIActions[i].GetTask()) && (bAchieveGoal || POIActions[i].GetServedGoals().Find(Goal) != INDEX_NONE))
					PlannerActions.Actions.Add(FSP_PlannerAction(POIActions[i].GetTask(), POIActions[i].GetWeight(LODLevel) * FMath::FRand(), bAchieveGoal));
			}
		}
	}

	// Sort newly added actions.
	PlannerActions.Actions.Sort(ActionSortFunctor());

#if SP_DEBUG
	// Check plan can be achieved.
	bool bCanAchievePlan = false;
	for (int i = 0; i < PlannerActions.Actions.Num(); i++)
	{
		if (PlannerActions.Actions[i].bAchievePlan)
		{
			bCanAchievePlan = true;
			break;
		}
	}

	if (!bCanAchievePlan)
		SP_LOG_SCREEN(Error, FColor::Red, "%s can't achieve goal %s", *CurrActionSet->GetName(), *Goal->GetName())
#endif

	return PlannerActions;
}

void USP_AIPlannerComponent::AskNewPlan(bool bInstantRequest)
{
	CurrentPlanIndex = -1;

	Super::AskNewPlan(bInstantRequest);
}

void USP_AIPlannerComponent::OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError)
{
	Super::OnPlanConstructionFailed_Implementation(PlanError);

	// Catch construction failed because of cooldowns.
	if (PlanError != ESP_PlanError::PE_ConstructionFailed)
		return;

	float MinCooldown = FLT_MAX;

	for (auto it = Cooldowns.begin(); it != Cooldowns.end(); ++it)
	{
		float Cooldown = it->Value - GetWorld()->GetTimeSeconds() - it->Key->GetCooldown();

		// Still in cooldown.
		if (Cooldown > 0.0f && Cooldown < MinCooldown)
			MinCooldown = Cooldown;
	}

	SP_CHECK(!FMath::IsNearlyEqual(MinCooldown, FLT_MAX), "Plan construction failed while no task are in cooldown.")

	GetWorld()->GetTimerManager().SetTimer(ConstructPlanTimer, [this]{	AskNewPlan(); }, MinCooldown, false);
}
bool USP_AIPlannerComponent::CancelPlan()
{
	if (!Super::CancelPlan() || CurrentPlanIndex == -1) // Plan not started.
		return false;

	SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), "Index [%d] out of range [0, %d[!", false, CurrentPlanIndex, Plan.Num())

	USP_AITask* CurrentTask = Cast<USP_AITask>(Plan[CurrentPlanIndex]);
	SP_RCHECK_NULLPTR(CurrentTask, false)

#if SP_DEBUG
	SP_RCHECK(CurrentTask->Cancel(this, TaskUserData.GetData()), "%s.Cancel() failed!", false, *CurrentTask->GetName())
#else
	CurrentTask->Cancel(this, TaskUserData.GetData());
#endif

	return true;
}

bool USP_AIPlannerComponent::OnActive_Internal_Implementation()
{
	if (!Super::OnActive_Internal_Implementation())
		return false;

	SetComponentTickEnabled(true);

	return true;
}
bool USP_AIPlannerComponent::OnInactive_Internal_Implementation()
{
	if(!Super::OnInactive_Internal_Implementation())
		return false;

	SetComponentTickEnabled(false);

	return true;
}

void USP_AIPlannerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;
	
	OnGoalChange.AddDynamic(this, &USP_AIPlannerComponent::OnGoalChange_Bind);
}
void USP_AIPlannerComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	// Server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
		return;

	OnGoalChange.RemoveDynamic(this, &USP_AIPlannerComponent::OnGoalChange_Bind);
}

void USP_AIPlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Computed by server only while owner is replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
	{
		// Simulated client.

		SetComponentTickEnabled(false);
		return;
	}
}
void USP_AIPlannerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Wait for computation.
	if (PlanState != ESP_PlanState::PS_Valid)
		return;

	ExecuteTask(DeltaTime);
}