#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>
#include <SPlanner/Base/Planner/SP_Planner.h>
#include <SPlanner/Base/Planner/SP_PlanState.h>
#include <SPlanner/Base/Actions/SP_PlannerActionSet.h>
#include <SPlanner/Base/Actions/SP_ActionSetComponent.h>
#include <SPlanner/Base/Zones/SP_PlannerLODComponent.h>

#include <SPlanner/AI/Controllers/SP_AIController.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>
#include <SPlanner/AI/POI/SP_POIActionSet.h>
#include <SPlanner/AI/POI/SP_POIZoneComponent.h>

#include <SPlanner/AI/Blackboard/SP_BlackboardAsset.h>
#include <SPlanner/AI/Blackboard/SP_BlackboardComponent.h>

#include <SPlanner/AI/Tasks/SP_Task.h>

#if SP_DEBUG_EDITOR

#include <SPlannerEditor/Miscs/SP_EditorSettings.h>

#endif

USP_AIPlannerComponent::USP_AIPlannerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Execute task in tick or check cooldowns.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	Blackboard = CreateDefaultSubobject<USP_BlackboardComponent>("Blackboard");
}

USP_BlackboardComponent* USP_AIPlannerComponent::GetBlackboard() const
{
	return Blackboard;
}

USP_ActionStep* USP_AIPlannerComponent::GetPrevActionStep() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")

	SP_RCHECK(CurrentPlanIndex < Plan.Num(), nullptr, "Index out of range!")

	// No previous action.
	if(CurrentPlanIndex <= 0)
		return nullptr;

	return Plan[CurrentPlanIndex - 1];
}
TArray<USP_ActionStep*> USP_AIPlannerComponent::GetPrevActionSteps() const
{
	TArray<USP_ActionStep*> Result;

	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Result, "Invalid plan: performing unsafe operation!")

	SP_RCHECK(CurrentPlanIndex < Plan.Num(), Result, "Index out of range!")
	
	// Plan started.
	if (CurrentPlanIndex != -1)
		Result.Append(Plan.GetData(), CurrentPlanIndex);

	return Result;
}
USP_ActionStep* USP_AIPlannerComponent::GetCurrentActionStep() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")

	SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), nullptr, "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())

	return Plan[CurrentPlanIndex];
}
USP_ActionStep* USP_AIPlannerComponent::GetNextActionStep() const
{
	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, nullptr, "Invalid plan: performing unsafe operation!")

	SP_RCHECK(CurrentPlanIndex < Plan.Num(), nullptr, "Index out of range!")

	// No other action left.
	if(CurrentPlanIndex + 1 >= Plan.Num())
		return nullptr;

	return Plan[CurrentPlanIndex + 1];
}
TArray<USP_ActionStep*> USP_AIPlannerComponent::GetNextActionSteps() const
{
	TArray<USP_ActionStep*> Result;

	// Plan is not thread safe while PlanState != ESP_PlanState::PS_Valid.
	SP_RCHECK(PlanState == ESP_PlanState::PS_Valid, Result, "Invalid plan: performing unsafe operation!")

	SP_RCHECK(CurrentPlanIndex < Plan.Num(), Result, "Index out of range!")

	// Plan started.
	if (CurrentPlanIndex != -1)
		Result.Append(Plan.GetData() + CurrentPlanIndex + 1, Plan.Num() - CurrentPlanIndex - 1);

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

	float Cooldown = Task->GetCooldown(LOD ? LOD->GetLevel() : -1.0f);

	// Never save task without cooldown.
	if(Cooldown <= 0.0f)
		return;

	float* const CooldownPtr = Cooldowns.Find(Task);

	// Add 0.001f to ensure float precision.

	if (CooldownPtr)
		*CooldownPtr = GetWorld()->GetTimeSeconds() + Cooldown + 0.001f;
	else
		Cooldowns.Add(Task, GetWorld()->GetTimeSeconds() + Cooldown + 0.001f);
}
bool USP_AIPlannerComponent::IsInCooldown(const USP_Task* Task) const
{
	SP_RCHECK_NULLPTR(Task, false)

	float TaskCooldown = Task->GetCooldown(LOD ? LOD->GetLevel() : -1.0f);

	// Never check task without cooldown.
	if(TaskCooldown <= 0.0f)
		return false;

	float Cooldown = GetCooldown(Task);

	return Cooldown > 0.0f && Cooldown >= TaskCooldown;
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

bool USP_AIPlannerComponent::BeginNextTask()
{
	SP_BENCHMARK_SCOPE(AIPC_BeginNextTask)

	// index == Plan.Num() will be checked to ask new plan.
	SP_RCHECK(CurrentPlanIndex + 1 >= 0 && CurrentPlanIndex + 1 <= Plan.Num(), false, "Index [%d] out of range [0, %d]!", (CurrentPlanIndex + 1), Plan.Num())

	++CurrentPlanIndex;
	
#if SP_DEBUG_EDITOR
	// Update debug keys.
	if(IsSelectedInEditor())
		USP_EditorSettings::IncrementTaskExecuteLogKey();
#endif

	// No other task left.
	if (CurrentPlanIndex == Plan.Num())
	{
		PlanState = ESP_PlanState::PS_Finished;

		AskNewPlan();
		return false;
	}

	USP_Task* CurrentTask = Cast<USP_Task>(GetCurrentActionStep());
	SP_RCHECK_NULLPTR(CurrentTask, false)

	// Reserve user data.
	if (uint32 UserDataSize = CurrentTask->GetUserDataSize())
		TaskUserData.Reserve(UserDataSize);

	// Can't begin task, Plan got invalid: ask a new one.
	if (!CurrentTask->Begin(*this, TaskUserData.GetData()))
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
	SP_BENCHMARK_SCOPE(AIPC_ExecuteTask)

	// Execute first task begin on main thread.
	if (CurrentPlanIndex == -1)
	{
		// Try to begin first task.
		if (!BeginNextTask())
			return;
	}

	USP_Task* CurrentTask = Cast<USP_Task>(GetCurrentActionStep());
	SP_CHECK_NULLPTR(CurrentTask)

	ESP_PlanExecutionState TickResult = CurrentTask->Tick(DeltaTime, *this, TaskUserData.GetData());

	// Process task result.
	if (TickResult == ESP_PlanExecutionState::PES_Running)
		return;

	// Always end task (Tick succeed or failed).
	if (!EndTask(CurrentTask) || TickResult == ESP_PlanExecutionState::PES_Failed)
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
bool USP_AIPlannerComponent::EndTask(USP_Task* Task)
{
	SP_BENCHMARK_SCOPE(AIPC_EndTask)

	SP_RCHECK_NULLPTR(Task, false)

	return Task->End(*this, TaskUserData.GetData());
}

FSP_PlannerActionSet USP_AIPlannerComponent::CreatePlannerActionSet(float LODLevel, bool* bCanBeAchievedPtr) const
{
	SP_BENCHMARK_SCOPE(AIPC_CreatePlannerActionSet)

	SP_RCHECK_NULLPTR(Goal, FSP_PlannerActionSet())
	SP_RCHECK_NULLPTR(ActionSet, FSP_PlannerActionSet())

	const USP_ActionSet* const CurrActionSet = ActionSet->GetActionSet(Goal);
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
	CurrentPlanIndex = -1;

	Super::AskNewPlan(bInstantRequest);
}

bool USP_AIPlannerComponent::ConstructPlan_Internal(FSP_PlannerActionSet& PlannerActions, TArray<USP_ActionStep*>& OutPlan, uint8 MaxDepth, float LODLevel) const
{
	return SP_Planner::LinearConstruct(SP_Planner::FSP_LinearConstructInfos{ *this, PlannerActions, OutPlan, MaxDepth, LODLevel });
}

void USP_AIPlannerComponent::OnPlanConstructionFailed_Implementation(ESP_PlanError PlanError)
{
	Super::OnPlanConstructionFailed_Implementation(PlanError);

	// Catch construction failed because of cooldowns.
	if (PlanError != ESP_PlanError::PE_CantBeAchieved)
		return;

	float MinCooldown = FLT_MAX;

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
	if (!Super::CancelPlan() || CurrentPlanIndex == -1) // Plan not started.
		return false;

	SP_RCHECK(CurrentPlanIndex >= 0 && CurrentPlanIndex < Plan.Num(), false, "Index [%d] out of range [0, %d[!", CurrentPlanIndex, Plan.Num())

	USP_Task* CurrentTask = Cast<USP_Task>(Plan[CurrentPlanIndex]);
	SP_RCHECK_NULLPTR(CurrentTask, false)

#if SP_DEBUG
	SP_RCHECK(CurrentTask->Cancel(*this, TaskUserData.GetData()), false, "%s.Cancel() failed!", *CurrentTask->GetName())
#else
	CurrentTask->Cancel(*this, TaskUserData.GetData());
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
	
	// Blackboard.
	SP_CHECK(BloackboardAsset, "AI Planner must have a Blackboard!")
	Blackboard->InitBlackboard(BloackboardAsset);
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
	if (ActionSet)
	{
		AActor* ActionSetOwner = ActionSet->GetOwner();

		if (ActionSetOwner)
			return ActionSetOwner->IsSelectedInEditor();
	}

	return false;
}
#endif