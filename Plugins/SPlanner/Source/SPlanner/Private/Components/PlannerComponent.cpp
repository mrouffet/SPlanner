#include <Components/PlannerComponent.h>

#include <Debug/Debug.h>

#include <Miscs/FlagHelper.h>
#include <Miscs/PlanConstructTask.h>

#include <Goal/Goal.h>

#include <Tasks/Task.h>
#include <Tasks/ActionSet.h>
#include <Tasks/POIActionSet.h>

#include <Components/TargetComponent.h>
#include <Components/POIComponent.h>
#include <Components/POIInteractZoneComponent.h>

USP_PlannerComponent::USP_PlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickInEditor = false;
	PrimaryComponentTick.bCanEverTick = true;

	// Must be init in cpp file (PS4 Standard).
	PlanState = ESP_PlanState::PS_Invalid;
}

void USP_PlannerComponent::SetGoal(USP_Goal* InGoal)
{
	if (Goal == InGoal)
		return;

	Goal = InGoal;

	AskNewPlan();
}

float USP_PlannerComponent::GetCooldown(const USP_Task* Task) const
{
	SP_RCHECK_NULLPTR(Task, -1.0f)

	const float* CooldownPtr = Cooldowns.Find(Task);

	return CooldownPtr ? GetWorld()->GetTimeSeconds() - *CooldownPtr : 0.0f;
}
void USP_PlannerComponent::SetCooldown(const USP_Task* Task)
{
	SP_CHECK_NULLPTR(Task)

	Cooldowns[Task] = GetWorld()->GetTimeSeconds() + Task->GetCooldown();
}
bool USP_PlannerComponent::IsInCooldown(const USP_Task* Task) const
{
	SP_RCHECK_NULLPTR(Task, false)

	if(Task->GetCooldown() <= 0.0f)
		return false;

	return GetCooldown(Task) < Task->GetCooldown();
}

USP_TargetComponent* USP_PlannerComponent::GetTarget() const
{
	return Target;
}

void USP_PlannerComponent::SetNewPlan(TArray<USP_Task*>&& InPlan)
{
#if SP_DEBUG
	if (PlanState == ESP_PlanState::PS_Valid)
		SP_LOG(Warning, "Set new plan while still valid!")

	// Log plan.
	if (IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Plan) && GetOwner()->IsSelected())
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

	if (InPlan.Num() != 0)
	{
		Plan = InPlan;
		PlanState = ESP_PlanState::PS_Valid;
	}
	else
		PlanState = ESP_PlanState::PS_Invalid;
}

bool USP_PlannerComponent::GetShuffledActions(TArray<FSP_Action>& ShuffledActions) const
{
	SP_RCHECK_NULLPTR(Goal, false)

	const USP_ActionSet* const* const ActionSetPtr = ActionsSets.Find(Goal);

	SP_RCHECK(ActionSetPtr || *ActionSetPtr, "No action set for goal [ %s ]", false , *Goal->GetName())

	// Pairs with random applied to action's weight.

	// Add all available actions.
	const TArray<FSP_Action>& Actions = (*ActionSetPtr)->GetActions();

	for (int i = 0; i < Actions.Num(); ++i)
	{
#if SP_DEBUG
		if (!Actions[i].Task)
		{
			SP_LOG(Error, "%s: Task [ %d ] nullptr!", *(*ActionSetPtr)->GetName(), i);
			continue;
		}
#endif

		if (!IsInCooldown(Actions[i].Task))
			ShuffledActions.Add(FSP_Action(Actions[i], FMath::FRand()));
	}


	// Add all available actions from POI.
	if (POIInteractZone)
	{
		for (int j = 0; j < POIInteractZone->GetPOIs().Num(); ++j)
		{
			SP_RCHECK(POIInteractZone->GetPOIs()[j], "%s: POI [ %d ] nullptr!", false, *GetName(), j)
			SP_RCHECK(POIInteractZone->GetPOIs()[j]->GetActionSet(), "%s: POI [ %d ] action set nullptr!", false, *GetName(), j)

			const TArray<FSP_POIAction>& POIActions = POIInteractZone->GetPOIs()[j]->GetActionSet()->GetActions();

			for (int i = 0; i < POIActions.Num(); ++i)
			{
#if SP_DEBUG
				if (!POIActions[i].Task)
				{
					SP_LOG(Error, "%s: POI Task [ %d ] nullptr!", *POIInteractZone->GetPOIs()[j]->GetActionSet()->GetName(), i);
					continue;
				}
#endif

				bool bAchieveGoal = POIActions[i].AchievedGoals.Find(Goal);

				if (!IsInCooldown(POIActions[i].Task) && (bAchieveGoal || POIActions[i].ServedGoals.Find(Goal)))
					ShuffledActions.Add(FSP_Action(POIActions[i].Task, POIActions[i].Weight * FMath::FRand(), bAchieveGoal));
			}
		}
	}


	// Sort using weight.
	struct AIActionSortFunctor
	{
		// Decreasing weight sort.
		bool operator()(const FSP_Action& lhs, const FSP_Action& rhs) const
		{
			return lhs.Weight > rhs.Weight;
		}
	};

	ShuffledActions.Sort(AIActionSortFunctor());

	return true;
}

void USP_PlannerComponent::AskNewPlan()
{
	Plan.Empty();

	// Wait for new plan computation.
	PlanState = ESP_PlanState::PS_Computing;

	CurrentPlanIndex = -1;

	// Queue plan construction in thread.
	(new FAutoDeleteAsyncTask<FSP_PlanConstructTask>(this))->StartBackgroundTask();
}
void USP_PlannerComponent::ConstructPlan()
{
	TArray<FSP_Action> ShuffledActions;
	
	// Query shuffled tasks.
#if SP_DEBUG
	// Query can failed while debug (error occured).
	if (!GetShuffledActions(ShuffledActions))
		return;
#else
	GetShuffledActions(ShuffledActions);
#endif

#if SP_DEBUG
	// Log available shuffled task.
	if (IS_FLAG_SET(USP_Settings::GetDebugMask(), ESP_DebugFlag::PD_Plan) && GetOwner()->IsSelected())
	{
		FString PlanDebugStr = "Move list: ";

		if (ShuffledActions.Num() != 0)
		{
			for (int i = 0; i < ShuffledActions.Num(); i++)
				PlanDebugStr += ShuffledActions[i].Task->GetName() + ", ";

			PlanDebugStr.RemoveAt(PlanDebugStr.Len() - 2, 2);
		}
		else
			PlanDebugStr += "null";

		SP_LOG_SCREEN_FULL(Display, USP_Settings::GetMoveListLogKey(), FColor::Yellow, USP_Settings::GetDebugScreenDisplayTime(), "%s", *PlanDebugStr)
	}

	// Check plan can be achieved.
	bool bCanAchievePlan = false;
	for (int i = 0; i < ShuffledActions.Num(); i++)
	{
		if (ShuffledActions[i].bAchieveGoal)
		{
			bCanAchievePlan = true;
			break;
		}
	}

	if(!bCanAchievePlan)
		SP_LOG_SCREEN(Error, FColor::Red, "%s can't achieve goal %s", *(*ActionsSets.Find(Goal))->GetName(), *Goal->GetName())
#endif

	TArray<USP_Task*> NewPlan;

	// Planner Computation.
	if (ConstructPlan_Internal(ShuffledActions, NewPlan))
		SetNewPlan(std::move(NewPlan));
	else
		PlanState = ESP_PlanState::PS_WaitForCooldown; // No plan found at this time, wait for cooldowns.
}
bool USP_PlannerComponent::ConstructPlan_Internal(const TArray<FSP_Action>& AvailableActions, TArray<USP_Task*>& OutPlan, int PlannerFlags, uint8 CurrDepth) const
{
	if (CurrDepth > USP_Settings::GetMaxPlannerDepth())
		return false;

	for (int i = CurrDepth; i < AvailableActions.Num(); ++i)
	{
		const FSP_Action& Action = AvailableActions[i];
		
		SP_RCHECK_NULLPTR(Action.Task, false)

		if (!Action.Task->PreCondition(this, PlannerFlags))
			continue;

		// Add task to current plan.
		OutPlan.Add(Action.Task);

		// Check if task achieve goal or plan with this task achieve goal.
		if (Action.bAchieveGoal || ConstructPlan_Internal(AvailableActions, OutPlan, Action.Task->PostCondition(this, PlannerFlags), CurrDepth + 1))
			return true;

		// Plan generation failed, remove this task from plan.
		OutPlan.RemoveAt(static_cast<int32>(CurrDepth)); // Remove last.
	}

	return false;
}

void USP_PlannerComponent::CheckCooldowns()
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

bool USP_PlannerComponent::BeginNextTask()
{
	// index == Plan.Num() will be checked to ask new plan.
	SP_RCHECK(CurrentPlanIndex + 1 >= 0 && CurrentPlanIndex + 1 <= Plan.Num(), "Index out of range!", false)

	++CurrentPlanIndex;
	
	// No other task left.
	if (CurrentPlanIndex == Plan.Num())
	{
		AskNewPlan();
		return false;
	}

	// Reserve user data.
	if (uint32 UserDataSize = Plan[CurrentPlanIndex]->GetUserDataSize())
		TaskUserData.Reserve(UserDataSize);

	// Can't begin task, Plan got invalid: ask a new one.
	if (Plan[CurrentPlanIndex]->Begin(this, TaskUserData.GetData()) != ESP_PlanExecutionState::PES_Succeed) 
	{
		AskNewPlan();
		return false;
	}

	return true;
}
bool USP_PlannerComponent::EndTask()
{
	SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), "Index out of range!", false)

	// Can't end task, Plan got invalid: ask a new one.
	if (Plan[CurrentPlanIndex]->End(this, TaskUserData.GetData()) != ESP_PlanExecutionState::PES_Succeed)
	{
		AskNewPlan();
		return false;
	}

	return true;
}
void USP_PlannerComponent::ExecuteTask(float DeltaTime)
{
	// Execute first task begin on main thread.
	if (CurrentPlanIndex == -1)
	{
		// Try to begin first task.
		if (!BeginNextTask())
			return;
	}

	ESP_PlanExecutionState TickResult = Plan[CurrentPlanIndex]->Tick(DeltaTime, this, TaskUserData.GetData());

	// Process task result.
	if (TickResult == ESP_PlanExecutionState::PES_Failed)			// Plan got invalid: ask a new one.
		AskNewPlan();
	else if (TickResult == ESP_PlanExecutionState::PES_Succeed)		// Task succeed.
	{
		if (!EndTask())
			return;

		BeginNextTask();
	}
}

void USP_PlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<USP_TargetComponent>(GetOwner()->GetComponentByClass(USP_TargetComponent::StaticClass()));

	// Computed by server while owner replicated.
	if (GetOwner()->GetIsReplicated() && GetOwnerRole() != ROLE_Authority)
	{
		SetComponentTickEnabled(false);
		return;
	}

	AskNewPlan();
}
void USP_PlannerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Wait for AI Planner computation */
	if (PlanState == ESP_PlanState::PS_Computing)
		return;

	// Check new task availability.
	if (PlanState == ESP_PlanState::PS_WaitForCooldown)
	{
		CheckCooldowns();
		return;
	}

	SP_CHECK(PlanState != ESP_PlanState::PS_Invalid, "Update invalid plan!")

	ExecuteTask(DeltaTime);
}