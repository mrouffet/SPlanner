// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Director/SP_Director.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#include <SPlanner/Base/Actions/SP_ActionSetComponent.h>

ASP_Director* ASP_Director::Instance = nullptr;

ASP_Director::ASP_Director(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASP_Director::PreInitializeComponents()
{
	Super::PreInitializeComponents();

#if SP_DEBUG
	if (Instance)
	{
#if SP_DEBUG_EDITOR
		// Editor call PreInitializeComponents() to preview an actor.
		SP_LOG(Error, "Bad Instance: Already set! If Director blueprint's has been previously edited ignore this error (won't trigger next play). Otherwise an other Director actor may already be in scene")
#else
		SP_LOG(Error, "Bad Instance: Already set! An other Director actor may already be in scene")
#endif
	}
#endif

	Instance = this;
}
void ASP_Director::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SP_CHECK(Instance == this, "Bad Instance: Try to reset an other instance! An other Director actor may already be in scene.")

	Instance = nullptr;
}

int ASP_Director::GetPlannerNum() const
{
	int Num = 0;

	for (auto it = GoalPlannersMap.begin(); it != GoalPlannersMap.end(); ++it)
		Num += it->Value.Num();

	return Num;
}
TArray<USP_PlannerComponent*> ASP_Director::QueryAllPlanners(bool bIncludeInactive) const
{
	TArray<USP_PlannerComponent*> Planners;

	for (auto it = GoalPlannersMap.begin(); it != GoalPlannersMap.end(); ++it)
		Planners.Append(it->Value);

	if(bIncludeInactive)
		Planners.Append(InactivePlanners);

	return Planners;
}
TArray<USP_PlannerComponent*> ASP_Director::QueryAllPlanners(bool bIncludeInactive)
{
	// Call const implementation.
	return static_cast<const ASP_Director*>(this)->QueryAllPlanners(bIncludeInactive);
}
const TArray<USP_PlannerComponent*>& ASP_Director::QueryAllPlannersWithGoal(USP_Goal* Goal, bool bIncludeInactive)
{
	TArray<USP_PlannerComponent*>* PlannersPtr = GoalPlannersMap.Find(Goal);

	TArray<USP_PlannerComponent*>& Planners = PlannersPtr ? *PlannersPtr : GoalPlannersMap.Add(Goal, {}); // Goal not previously registered: add to map.
	
	if (bIncludeInactive)
	{
		for (int i = 0; i < InactivePlanners.Num(); ++i)
		{
			if (InactivePlanners[i]->GetGoal() == Goal)
				Planners.Add(InactivePlanners[i]);
		}
	}

	return Planners;
}

ASP_Director* ASP_Director::GetInstance()
{
	SP_SRCHECK(Instance, nullptr, "Instance nullptr! AIDirector actor must be put in scene.")

	return Instance;
}

void ASP_Director::Register(USP_PlannerComponent* InPlanner)
{
	SP_SCHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.")

	Instance->OnRegister_Internal(InPlanner);
}
void ASP_Director::UnRegister(USP_PlannerComponent* InPlanner)
{
	SP_SCHECK(Instance, "Instance nullptr! AIDirector actor must be put in scene.")

	Instance->OnUnRegister_Internal(InPlanner);
}

bool ASP_Director::TryRegister(USP_PlannerComponent* InPlanner)
{
	if(!Instance)
		return false;

	Instance->OnRegister_Internal(InPlanner);

	return true; // Unreal can't assume BlueprintNativeEvent with return type. Let's assume OnRegister_Internal succeeded.
}
bool ASP_Director::TryUnRegister(USP_PlannerComponent* InPlanner)
{
	if (!Instance)
		return false;

	Instance->OnUnRegister_Internal(InPlanner);

	return true; // Unreal can't assume BlueprintNativeEvent with return type. Let's assume OnUnRegister_Internal succeeded.
}

void ASP_Director::OnRegister_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)

#if SP_DEBUG
	// Check not already registered.
	TArray<USP_PlannerComponent*>* PlannersPtr = GoalPlannersMap.Find(InPlanner->GetGoal());

	if(PlannersPtr)
		SP_CHECK(PlannersPtr->Find(InPlanner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered with goal [ %s ]!",
			*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), *(InPlanner->GetGoal() ? InPlanner->GetGoal()->GetName() : FString("None")))
		
	SP_CHECK(InactivePlanners.Find(InPlanner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())
#endif

	// Always register as inactive.
	InactivePlanners.Add(InPlanner);
	
	InPlanner->OnActive.AddDynamic(this, &ASP_Director::OnRegistedPlannerActive);
	InPlanner->OnInactive.AddDynamic(this, &ASP_Director::OnRegistedPlannerInactive);
	InPlanner->OnGoalChange.AddDynamic(this, &ASP_Director::OnRegistedPlannerGoalChange);

	OnRegister.Broadcast(InPlanner);
}
void ASP_Director::OnUnRegister_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)

	if (InPlanner->GetPlanState() == ESP_PlanState::PS_Inactive)
	{
		SP_CHECK(InactivePlanners.Find(InPlanner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered as inactive!", *InPlanner->GetName(), *InPlanner->GetOwner()->GetName())
		InactivePlanners.Remove(InPlanner);
	}
	else
	{
		TArray<USP_PlannerComponent*>* PlannersPtr = GoalPlannersMap.Find(InPlanner->GetGoal());
		SP_CHECK_NULLPTR(PlannersPtr) // Should never be nullptr (previously added in OnRegistedPlannerActive() or OnRegistedPlannerGoalChange).

		SP_CHECK(PlannersPtr->Find(InPlanner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered with goal [ %s ]!",
			*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), *(InPlanner->GetGoal() ? InPlanner->GetGoal()->GetName() : FString("None")))
		PlannersPtr->Remove(InPlanner);
	}

	InPlanner->OnActive.RemoveDynamic(this, &ASP_Director::OnRegistedPlannerActive);
	InPlanner->OnInactive.RemoveDynamic(this, &ASP_Director::OnRegistedPlannerInactive);
	InPlanner->OnGoalChange.RemoveDynamic(this, &ASP_Director::OnRegistedPlannerGoalChange);

	OnUnRegister.Broadcast(InPlanner);
}

void ASP_Director::OnRegistedPlannerActive(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(InactivePlanners.Find(InPlanner) != INDEX_NONE, "InPlanner not previously inactive")
	
	InactivePlanners.Remove(InPlanner);

	TArray<USP_PlannerComponent*>* PlannersPtr = GoalPlannersMap.Find(InPlanner->GetGoal());
	
	if (PlannersPtr)
	{
		SP_CHECK(PlannersPtr->Find(InPlanner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered with goal [ %s ]!",
			*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), *(InPlanner->GetGoal() ? InPlanner->GetGoal()->GetName() : FString("None")))
		PlannersPtr->Add(InPlanner);
	}
	else
		GoalPlannersMap.Add(InPlanner->GetGoal(), { InPlanner });
}
void ASP_Director::OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(InactivePlanners.Find(InPlanner) == INDEX_NONE, "InPlanner already registered as inactive")

	TArray<USP_PlannerComponent*>* PlannersPtr = GoalPlannersMap.Find(InPlanner->GetGoal());

	SP_CHECK_NULLPTR(PlannersPtr) // Should never be nullptr (previously added in OnRegistedPlannerActive or OnRegistedPlannerGoalChange).
	SP_CHECK(PlannersPtr->Find(InPlanner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered with goal [ %s ]!",
		*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), *(InPlanner->GetGoal() ? InPlanner->GetGoal()->GetName() : FString("None")))
	
	PlannersPtr->Remove(InPlanner);
	InactivePlanners.Add(InPlanner);
}
void ASP_Director::OnRegistedPlannerGoalChange(USP_PlannerComponent* InPlanner, USP_Goal* OldGoal, USP_Goal* NewGoal)
{
	SP_CHECK_NULLPTR(InPlanner)

	// Inactive planner.
	if(InPlanner->GetPlanState() == ESP_PlanState::PS_Inactive)
		return;

	// Find previous goal registered.
	TArray<USP_PlannerComponent*>* PlannersPtr = GoalPlannersMap.Find(OldGoal);
	
	SP_CHECK_NULLPTR(PlannersPtr) // Should never be nullptr (previously added in OnRegister_Internal()).
	SP_CHECK(PlannersPtr->Find(InPlanner) != INDEX_NONE, "Planner [ %s ] in [ %s ] not previously registered with goal [ %s ]!",
		*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), (OldGoal ? *OldGoal->GetName() : *FString("None")))

	PlannersPtr->Remove(InPlanner);

	// Register planner with new goal.
	PlannersPtr = GoalPlannersMap.Find(NewGoal);

	if (PlannersPtr)
	{
		SP_CHECK(PlannersPtr->Find(InPlanner) == INDEX_NONE, "Planner [ %s ] in [ %s ] already registered with goal [ %s ]!",
			*InPlanner->GetName(), *InPlanner->GetOwner()->GetName(), (NewGoal ? *NewGoal->GetName() : *FString("None")))
		PlannersPtr->Add(InPlanner);
	}
	else
		GoalPlannersMap.Add(NewGoal, { InPlanner });
}

void ASP_Director::SetAllSelectedPlannerGoal(USP_Goal* NewGoal, bool bApplyToAllIfNoSelected)
{
	SP_CHECK_NULLPTR(NewGoal)

#if !SP_DEBUG
	// The method is not defined using WITH_EDITOR to be able to call it in BP as debug.

	SP_LOG(Warning, "This method should only be called in debug (require SP_DEBUG)!")
	return;

#else

#if SP_DEBUG_EDITOR

	TArray<USP_PlannerComponent*> AllPlanners;

	for (auto it = GoalPlannersMap.begin(); it != GoalPlannersMap.end(); ++it)
	{
		for (int i = 0; i < it->Value.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(it->Value[i])

			if (it->Value[i]->IsSelected())
			{
				it->Value[i]->SetGoal(NewGoal, true);
				bApplyToAllIfNoSelected = false; // Almost one is selected.
			}
			else if (bApplyToAllIfNoSelected)
				AllPlanners.Add(it->Value[i]);
		}
	}

	if (bApplyToAllIfNoSelected)
	{
		for (int i = 0; i < AllPlanners.Num(); ++i)
			AllPlanners[i]->SetGoal(NewGoal, true);
	}
#else

	TArray<USP_PlannerComponent*> AllPlanners = QueryAllPlanners();
	
	for (int i = 0; i < AllPlanners.Num(); ++i)
		AllPlanners[i]->SetGoal(NewGoal, true);

#endif


	SP_LOG_SCREEN(Display, FColor::Magenta, "Set Goal: %s", *NewGoal->GetName())

#endif
}