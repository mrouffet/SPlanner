// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>
#include <SPlanner/Base/Planner/SP_Planner.h>
#include <SPlanner/Base/Planner/SP_PlanState.h>
#include <SPlanner/Base/Actions/SP_PlannerActionSet.h>
#include <SPlanner/Base/Zones/SP_PlannerLODComponent.h>

#include <SPlanner/AI/Controllers/SP_AIController.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>
#include <SPlanner/AI/POI/SP_POIActionSet.h>
#include <SPlanner/AI/POI/SP_POIZoneComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Tasks/SP_Task.h>

#if SP_DEBUG_EDITOR

#include <SPlannerEditor/Miscs/SP_EditorSettings.h>

#endif

USP_AIPlannerComponent::USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Execute task in tick or check cooldowns.
	PrimaryComponentTick.bCanEverTick = true;

	PlanGenInfosClass = USP_AIPlanGenInfos::StaticClass();
}

USP_Task* USP_AIPlannerComponent::GetPrevTask() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), nullptr, "Index out of range!")

	// No previous action.
	if(CurrentPlanIndex <= 0)
		return nullptr;

	return Cast<USP_Task>(Plan[CurrentPlanIndex - 1]);
}
TArray<USP_Task*> USP_AIPlannerComponent::GetPrevTasks() const
{
	TArray<USP_Task*> Result;

	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Result, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), Result, "Index out of range!")

	Result.Append(reinterpret_cast<USP_Task* const*>(Plan.GetData()), CurrentPlanIndex);

	return Result;
}
USP_Task* USP_AIPlannerComponent::GetCurrentTask() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), nullptr, "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())

	return Cast<USP_Task>(Plan[CurrentPlanIndex]);
}
USP_Task* USP_AIPlannerComponent::GetNextTask() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), nullptr, "Index out of range!")

	// No other action left.
	if(CurrentPlanIndex + 1 >= Plan.Num())
		return nullptr;

	return Cast<USP_Task>(Plan[CurrentPlanIndex + 1]);
}
TArray<USP_Task*> USP_AIPlannerComponent::GetNextTasks() const
{
	TArray<USP_Task*> Result;

	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Result, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), Result, "Index out of range!")

	Result.Append(reinterpret_cast<USP_Task* const*>(Plan.GetData()) + CurrentPlanIndex + 1, Plan.Num() - CurrentPlanIndex - 1);

	return Result;
}

float USP_AIPlannerComponent::GetCooldown(const USP_Task* Task) const
{
	SP_RCHECK_NULLPTR(Task, -1.0f)

	const float* const CooldownPtr = Cooldowns.Find(Task);
	return CooldownPtr ? *CooldownPtr - GetWorld()->GetTimeSeconds() : -1.0f;
}
void USP_AIPlannerComponent::SetCooldown(const USP_Task* Task)
{
	SP_CHECK_NULLPTR(Task)

	float Cooldown = Task->GetCooldown(GetLODLevel());

	// Never save task without cooldown.
	if(Cooldown <= 0.0f)
		return;
	
	// Add 0.001f to ensure float precision.
	Cooldowns.FindOrAdd(Task) = GetWorld()->GetTimeSeconds() + Cooldown + 0.001f;
}
bool USP_AIPlannerComponent::IsInCooldown(const USP_Task* Task) const
{
	SP_RCHECK_NULLPTR(Task, false)

	return GetCooldown(Task) > 0.0f;
}

void USP_AIPlannerComponent::Notify(ESP_AIPlannerNotify Notify)
{
	OnNotify.Broadcast(this, Notify);
	OnNotifyTask.Broadcast(this, Notify, TaskInfos);
}

ASP_AIController* USP_AIPlannerComponent::GetController() const
{
	ASP_AIController* const AIController = Cast<ASP_AIController>(GetOwner());

	SP_RCHECK(AIController, nullptr, "AIController nullptr! AIPlanner is not attached to a SP_AIController")

	return AIController;
}
APawn* USP_AIPlannerComponent::GetPawn() const
{
	if (ASP_AIController* const AIController = GetController())
		return AIController->GetPawn();

	return nullptr;
}

void USP_AIPlannerComponent::ExecuteTask(float DeltaTime)
{
	SP_BENCHMARK_SCOPE(AIPC_ExecuteTask)

	USP_Task* CurrentTask = GetCurrentTask();
	SP_CHECK_NULLPTR(CurrentTask)

	if(!TaskInfos)
		TaskInfos = CurrentTask->InstantiateInfos();

	ESP_PlanExecutionState TickResult = CurrentTask->Tick(DeltaTime, *this, TaskInfos);

	// Wait for either fail or success.
	if (TickResult == ESP_PlanExecutionState::PES_Running)
		return;

	// Reset task infos.
	TaskInfos = nullptr;

	// Process task result.
	if (TickResult == ESP_PlanExecutionState::PES_Failed)
	{
		// Set cooldown.
		if (CurrentTask->GetUseCooldownOnFailed())
			SetCooldown(CurrentTask);

		// Plan got invalid: ask a new one.
		PlanState = ESP_PlanState::PS_Invalid;

		AskNewPlan();
	}
	else // Succeed.
	{
		// Set cooldown.
		SetCooldown(CurrentTask);

		// No other task left.
		if (++CurrentPlanIndex == Plan.Num())
		{
			PlanState = ESP_PlanState::PS_Finished;

			AskNewPlan();
		}
	}
}

FSP_PlannerActionSet USP_AIPlannerComponent::CreatePlannerActionSet(float LODLevel, bool* bCanBeAchievedPtr) const
{
	SP_BENCHMARK_SCOPE(AIPC_CreatePlannerActionSet)

	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(Blackboard, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = Blackboard->GetActionSet(Goal);
	SP_RCHECK_NULLPTR(CurrActionSet, FSP_PlannerActionSet())

	struct CooldownPredicate
	{
		const USP_AIPlannerComponent* Planner = nullptr;

		bool operator()(const FSP_Action& Action) const
		{
			SP_SRCHECK_NULLPTR(Planner, false)

			return !Planner->IsInCooldown(Cast<USP_Task>(Action.Step));
		}
	};

#if PLATFORM_WINDOWS
	FSP_PlannerActionSet PlannerActions = CurrActionSet->Shuffle(LODLevel, CooldownPredicate{ this }, bCanBeAchievedPtr);
#else
	FSP_PlannerActionSet PlannerActions = FSP_PlannerActionSet::Make(CurrActionSet, LODLevel, CooldownPredicate{ this }, bCanBeAchievedPtr);
#endif

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

				// Add to actions.
				if (!IsInCooldown(POIActions[i].GetTask()) && (bAchieveGoal || POIActions[i].GetServedGoals().Find(Goal) != INDEX_NONE))
				{
					PlannerActions.Actions.Add(FSP_PlannerAction::Make(&POIActions[i], LODLevel, bAchieveGoal));

					// Update goal achieve.
					if (bCanBeAchievedPtr && !*bCanBeAchievedPtr)
						*bCanBeAchievedPtr = bAchieveGoal;
				}
			}
		}
	}

	// Plan can't be achieve: save performance.
	if(bCanBeAchievedPtr && !*bCanBeAchievedPtr)
		return PlannerActions;

	// Sort newly added actions.
	PlannerActions.Actions.Sort(FSP_PlannerActionSortFunctor());

	return PlannerActions;
}

void USP_AIPlannerComponent::AskNewPlan(bool bInstantRequest)
{
	CurrentPlanIndex = 0;

	Super::AskNewPlan(bInstantRequest);
}

bool USP_AIPlannerComponent::ConstructPlan_Internal(FSP_PlannerActionSet& PlannerActions,
	TArray<USP_ActionStep*>& OutPlan,
	USP_PlanGenInfos* PlanGenInfos,
	uint8 MaxDepth,
	float LODLevel) const
{
	SP_RCHECK(Cast<USP_AIPlanGenInfos>(PlanGenInfos), false, "AI PlanGenInfos nullptr! PlanGenInfos must be a USP_AIPlanGenInfos!")

	return SP_Planner::LinearConstruct(SP_Planner::FSP_LinearConstructInfos{ this, PlannerActions, OutPlan, PlanGenInfos, MaxDepth, LODLevel });
}

void USP_AIPlannerComponent::OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError)
{
	Super::OnPlanConstructionFailed_Implementation(PlanError);

	// Catch construction failed because of cooldowns.
	if (PlanError != ESP_PlanError::PE_CantBeAchieved)
		return;

	float MinCooldown = FLT_MAX;

	// Check among all cooldown registered, not only action in current ActionSet: POI Task may be in cooldown.
	for (auto it = Cooldowns.begin(); it != Cooldowns.end(); ++it)
	{
		float Cooldown = it->Value - GetWorld()->GetTimeSeconds();

		// Still in cooldown.
		if (Cooldown > 0.0f && Cooldown < MinCooldown)
			MinCooldown = Cooldown;
	}

	SP_CHECK(!FMath::IsNearlyEqual(MinCooldown, FLT_MAX), "Plan construction failed while no task are in cooldown.")

	GetWorld()->GetTimerManager().SetTimer(ConstructPlanTimer, [this]{	AskNewPlan(); }, MinCooldown, false);
}
bool USP_AIPlannerComponent::CancelPlan()
{
	if (!Super::CancelPlan())
		return false;

	// Task started.
	if (TaskInfos)
	{
		SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), false, "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())

		USP_Task* CurrentTask = Cast<USP_Task>(Plan[CurrentPlanIndex]);
		SP_RCHECK_NULLPTR(CurrentTask, false)

		CurrentTask->Cancel(*this, TaskInfos);
		TaskInfos = nullptr;
	}

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

#if WITH_EDITOR
bool USP_AIPlannerComponent::IsSelectedInEditor() const
{
	// This selected (usually Controller).
	if (Super::IsSelectedInEditor())
		return true;

	// ActionSet owner selected (usually Character).
	if (APawn* Pawn = GetPawn())
		return Pawn->IsSelectedInEditor();

	return false;
}
#endif