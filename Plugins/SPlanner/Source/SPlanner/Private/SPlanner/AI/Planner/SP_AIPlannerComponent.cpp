// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>
#include <SPlanner/Base/Planner/SP_PlanState.h>
#include <SPlanner/Base/Planner/SP_PlannerEngine.h>
#include <SPlanner/Base/Action/SP_PlannerActionSet.h>

#include <SPlanner/AI/Controller/SP_AIController.h>

#include <SPlanner/AI/POI/SP_POIAction.h>
#include <SPlanner/AI/POI/SP_POIActionSet.h>
#include <SPlanner/AI/POI/SP_POIComponent.h>
#include <SPlanner/AI/POI/SP_POIZoneComponent.h>

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

#include <SPlanner/AI/Task/SP_TaskStep.h>
#include <SPlanner/AI/LOD/SP_AILODComponent.h>

#if SP_DEBUG_EDITOR

#include <SPlannerEditor/Misc/SP_EditorSettings.h>

#endif

USP_AIPlannerComponent::USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Execute task in tick or check cooldowns.
	PrimaryComponentTick.bCanEverTick = true;

	PlanGenInfosClass = USP_AIPlanGenInfos::StaticClass();
}

float USP_AIPlannerComponent::GetLODLevel() const
{
	if (LOD)
		return LOD->GetLODLevel();

	return -1.0f;
}
void USP_AIPlannerComponent::SetLOD(USP_AILODComponent* NewLOD)
{
	if (LOD)
	{
		LOD->OnActive.RemoveDynamic(this, &USP_AIPlannerComponent::OnActiveLOD);
		LOD->OnInactive.RemoveDynamic(this, &USP_AIPlannerComponent::OnInactiveLOD);
	}

	LOD = NewLOD;

	// Can be nullptr to reset component.
	if (LOD)
	{
		LOD->OnActive.AddDynamic(this, &USP_AIPlannerComponent::OnActiveLOD);
		LOD->OnInactive.AddDynamic(this, &USP_AIPlannerComponent::OnInactiveLOD);
	}
}

USP_TaskStep* USP_AIPlannerComponent::GetPrevTask() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), nullptr, "Index out of range!")

	// No previous action.
	if(CurrentPlanIndex <= 0)
		return nullptr;

	return Cast<USP_TaskStep>(Plan[CurrentPlanIndex - 1]);
}
TArray<USP_TaskStep*> USP_AIPlannerComponent::GetPrevTasks() const
{
	TArray<USP_TaskStep*> Result;

	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Result, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), Result, "Index out of range!")

	Result.Append(reinterpret_cast<USP_TaskStep* const*>(Plan.GetData()), CurrentPlanIndex);

	return Result;
}
USP_TaskStep* USP_AIPlannerComponent::GetCurrentTask() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), nullptr, "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())

	return Cast<USP_TaskStep>(Plan[CurrentPlanIndex]);
}
USP_TaskStep* USP_AIPlannerComponent::GetNextTask() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), nullptr, "Index out of range!")

	// No other action left.
	if(CurrentPlanIndex + 1 >= Plan.Num())
		return nullptr;

	return Cast<USP_TaskStep>(Plan[CurrentPlanIndex + 1]);
}
TArray<USP_TaskStep*> USP_AIPlannerComponent::GetNextTasks() const
{
	TArray<USP_TaskStep*> Result;

	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Result, "Invalid plan: performing unsafe operation!")
	SP_RCHECK(CurrentPlanIndex < Plan.Num(), Result, "Index out of range!")

	Result.Append(reinterpret_cast<USP_TaskStep* const*>(Plan.GetData()) + CurrentPlanIndex + 1, Plan.Num() - CurrentPlanIndex - 1);

	return Result;
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

	USP_TaskStep* CurrentTask = GetCurrentTask();
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
		// Plan got invalid: ask a new one.
		PlanState = ESP_PlanState::PS_Invalid;

		AskNewPlan();
	}
	else if (++CurrentPlanIndex >= Plan.Num()) // Succeed and No other task left.
	{
		PlanState = ESP_PlanState::PS_Finished;

		AskNewPlan();
	}
}

void USP_AIPlannerComponent::OnActiveLOD()
{
	SetEnableBehavior(true);
}
void USP_AIPlannerComponent::OnInactiveLOD()
{
	SetEnableBehavior(false);
}

FSP_PlannerActionSet USP_AIPlannerComponent::CreatePlannerActionSet(bool* bCanBeAchievedPtr) const
{
	SP_BENCHMARK_SCOPE(AIPC_CreatePlannerActionSet)

	FSP_PlannerActionSet PlannerActions = Super::CreatePlannerActionSet(bCanBeAchievedPtr);

	// Add all available actions from POI.
	if (POIZone)
	{
		for (int j = 0; j < POIZone->GetPOIs().Num(); ++j)
		{
			SP_CCHECK(POIZone->GetPOIs()[j], "%s: POI [%d] nullptr!", false, *GetName(), j)
			SP_CCHECK(POIZone->GetPOIs()[j]->GetActionSet(), "%s: POI [%s] action set nullptr!",
				false, *GetName(), *POIZone->GetPOIs()[j]->GetOwner()->GetName())

			const TArray<USP_POIAction*>& POIActions = POIZone->GetPOIs()[j]->GetActionSet()->GetActions();

			for (int i = 0; i < POIActions.Num(); ++i)
			{
				SP_CCHECK(POIActions[i], "%s: POI [ %d ] nullptr!", *POIZone->GetPOIs()[j]->GetActionSet()->GetName(), i)
				SP_CCHECK(POIActions[i]->GetTask(), "%s: POI \"%s\" [%d] Task nullptr!",
					*POIZone->GetPOIs()[j]->GetActionSet()->GetName(), *POIActions[i]->GetName(), i)

				// Add to actions.
				if (POIActions[i]->IsAvailable(this))
				{
					bool bAchieveGoal = POIActions[i]->IsGoalAchieved(Goal);

					PlannerActions.Actions.Add(FSP_PlannerAction::Make(this, POIActions[i], bAchieveGoal));

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

bool USP_AIPlannerComponent::ConstructPlan_Internal(USP_PlanGenInfos* Infos) const
{
	SP_RCHECK(Cast<USP_AIPlanGenInfos>(Infos), false, "AI Infos nullptr! Infos must be a USP_AIPlanGenInfos!")

	return SP_PlannerEngine::LinearConstruct(Infos);
}

void USP_AIPlannerComponent::OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError)
{
	Super::OnPlanConstructionFailed_Implementation(PlanError);

	// TODO: FIX.
	//// Catch construction failed because of cooldowns.
	//if (PlanError != ESP_PlanError::PE_CantBeAchieved)
	//	return;

	//float MinCooldown = FLT_MAX;

	//// Check among all cooldown registered, not only action in current ActionSet: POI Task may be in cooldown.
	//for (auto it = Cooldowns.begin(); it != Cooldowns.end(); ++it)
	//{
	//	float Cooldown = it->Value - GetWorld()->GetTimeSeconds();

	//	// Still in cooldown.
	//	if (Cooldown > 0.0f && Cooldown < MinCooldown)
	//		MinCooldown = Cooldown;
	//}

	//SP_CHECK(!FMath::IsNearlyEqual(MinCooldown, FLT_MAX), "Plan construction failed while no task are in cooldown.")

	//GetWorld()->GetTimerManager().SetTimer(ConstructPlanTimer, [this]{	AskNewPlan(); }, MinCooldown, false);
	GetWorld()->GetTimerManager().SetTimer(ConstructPlanTimer, [this] {	AskNewPlan(); }, 0.5f, false);
}
bool USP_AIPlannerComponent::CancelPlan()
{
	if (!Super::CancelPlan())
		return false;

	// Task started.
	if (TaskInfos)
	{
		SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), false, "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())

		USP_TaskStep* CurrentTask = Cast<USP_TaskStep>(Plan[CurrentPlanIndex]);
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

float USP_AIPlannerComponent::QueryTimeBeforeConstructPlan_Implementation() const
{
	if (LOD)
	{
		SP_RCHECK(LOD->IsLODActive(), Super::QueryTimeBeforeConstructPlan_Implementation(), "LOD inactive!")
		return LOD->GetTimeBeforeConstructPlan();
	}

	return Super::QueryTimeBeforeConstructPlan_Implementation();
}
int USP_AIPlannerComponent::QueryMaxPlannerDepth_Implementation() const
{
	if (LOD && LOD->GetLODLevel() > 0.0f)
		return static_cast<int>(LOD->GetMaxPlannerDepth());

	return Super::QueryMaxPlannerDepth_Implementation();
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