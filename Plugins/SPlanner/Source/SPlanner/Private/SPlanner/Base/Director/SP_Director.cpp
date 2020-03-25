// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Director/SP_Director.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

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

	for (int i = 0; i < Goals.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(Goals[i])
		Goals[i]->Reset();
	}

	Instance = nullptr;
}

int ASP_Director::GetPlannerNum(bool bIncludeInactive) const
{
	int PlannerNum = ActivePlanners.Num();

	if(bIncludeInactive)
		PlannerNum += InactivePlanners.Num();

	return PlannerNum;
}

const TArray<USP_PlannerComponent*>& ASP_Director::GetActivePlanners() const
{
	return ActivePlanners;
}
const TArray<USP_PlannerComponent*>& ASP_Director::GetInactivePlanners() const
{
	return InactivePlanners;
}
TArray<USP_PlannerComponent*> ASP_Director::GetAllPlanners() const
{
	TArray<USP_PlannerComponent*> AllPlanners;
	AllPlanners.Reserve(GetPlannerNum(true));

	AllPlanners.Append(ActivePlanners);
	AllPlanners.Append(InactivePlanners);

	return AllPlanners;
}

ASP_Director* ASP_Director::GetInstance()
{
	return Instance;
}

void ASP_Director::RegisterPlanner(USP_PlannerComponent* InPlanner)
{
	SP_SCHECK(Instance, "Instance nullptr! Director actor must be put in scene.")

	Instance->RegisterPlanner_Internal(InPlanner);
}
void ASP_Director::UnRegisterPlanner(USP_PlannerComponent* InPlanner)
{
	SP_SCHECK(Instance, "Instance nullptr! Director actor must be put in scene.")

	Instance->UnRegisterPlanner_Internal(InPlanner);
}

void ASP_Director::RegisterPlanner_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(ActivePlanners.Find(InPlanner) == INDEX_NONE || InactivePlanners.Find(InPlanner) == INDEX_NONE,
		"Planner [%s] already registered in director!", *InPlanner->GetOwner()->GetName())

	// Always register as inactive.
	InactivePlanners.Add(InPlanner);

	InPlanner->OnActive.AddDynamic(this, &ASP_Director::OnRegistedPlannerActive);
	InPlanner->OnInactive.AddDynamic(this, &ASP_Director::OnRegistedPlannerInactive);

	OnRegisterPlanner.Broadcast(InPlanner);
}
void ASP_Director::UnRegisterPlanner_Internal_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(ActivePlanners.Find(InPlanner) != INDEX_NONE || InactivePlanners.Find(InPlanner) != INDEX_NONE,
		"Planner [%s] not previously registered in director!", *InPlanner->GetOwner()->GetName())

	if (InPlanner->GetPlanState() == ESP_PlanState::PS_Inactive)
	{
		SP_CHECK(InactivePlanners.Find(InPlanner) != INDEX_NONE,
			"Planner [%s] not previously registered as inactive in director!", *InPlanner->GetOwner()->GetName())

		InactivePlanners.Remove(InPlanner);
	}
	else
	{
		SP_CHECK(ActivePlanners.Find(InPlanner) != INDEX_NONE,
			"Planner [%s] not previously registered as active in director!", *InPlanner->GetOwner()->GetName())

		ActivePlanners.Remove(InPlanner);
	}

	InPlanner->OnActive.RemoveDynamic(this, &ASP_Director::OnRegistedPlannerActive);
	InPlanner->OnInactive.RemoveDynamic(this, &ASP_Director::OnRegistedPlannerInactive);

	OnUnRegisterPlanner.Broadcast(InPlanner);
}

void ASP_Director::OnRegistedPlannerActive(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(InactivePlanners.Find(InPlanner) != INDEX_NONE, "Planner not previously as inactive")

	ActivePlanners.Add(InPlanner);
	InactivePlanners.Remove(InPlanner);
}
void ASP_Director::OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(ActivePlanners.Find(InPlanner) != INDEX_NONE, "Planner not previously as active")

	InactivePlanners.Add(InPlanner);
	ActivePlanners.Remove(InPlanner);
}

void ASP_Director::RegisterGoal(USP_Goal* InGoal)
{
	SP_SCHECK(Instance, "Instance nullptr! Director actor must be put in scene.")

	Instance->RegisterGoal_Internal(InGoal);
}
void ASP_Director::UnRegisterGoal(USP_Goal* InGoal)
{
	SP_SCHECK(Instance, "Instance nullptr! Director actor must be put in scene.")

	Instance->UnRegisterGoal_Internal(InGoal);
}

void ASP_Director::RegisterGoal_Internal_Implementation(USP_Goal* InGoal)
{
	SP_CHECK_NULLPTR(InGoal)
	SP_CHECK(Goals.Find(InGoal) == INDEX_NONE, "Goal [%s] already registered!", *InGoal->GetName())

	Goals.Add(InGoal);
}
void ASP_Director::UnRegisterGoal_Internal_Implementation(USP_Goal* InGoal)
{
	SP_CHECK_NULLPTR(InGoal)
	SP_CHECK(Goals.Find(InGoal) != INDEX_NONE, "Goal [%s] not previously registered!", *InGoal->GetName())

	Goals.Remove(InGoal);
}

void ASP_Director::SetAllSelectedPlannerGoal(USP_Goal* NewGoal, bool bApplyToAllIfNoSelected)
{
	SP_CHECK_NULLPTR(NewGoal)

#if SP_DEBUG_EDITOR

	for (int i = 0; i < ActivePlanners.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(ActivePlanners[i])

		if (ActivePlanners[i]->IsSelected())
		{
			ActivePlanners[i]->SetGoal(NewGoal, true);
			bApplyToAllIfNoSelected = false; // At least one is selected.
		}
	}

	if (bApplyToAllIfNoSelected)
	{
		for (int i = 0; i < ActivePlanners.Num(); ++i)
		{
			SP_CCHECK_NULLPTR(ActivePlanners[i])
			ActivePlanners[i]->SetGoal(NewGoal, true);
		}
	}
#else

	for (int i = 0; i < ActivePlanners.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(ActivePlanners[i])
			ActivePlanners[i]->SetGoal(NewGoal, true);
	}

#endif

	SP_LOG_SCREEN(Display, FColor::Magenta, "Set Goal: %s", *NewGoal->GetName())
}