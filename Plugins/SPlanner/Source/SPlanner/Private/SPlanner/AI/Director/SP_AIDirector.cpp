// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Director/SP_AIDirector.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Formation/SP_FormationSet.h>

ASP_AIDirector::ASP_AIDirector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

const TArray<USP_FormationSet*>& ASP_AIDirector::GetFormationSets() const
{
	return FormationSets;
}

void ASP_AIDirector::OnRegistedPlannerActive(USP_PlannerComponent* InPlanner)
{
	Super::OnRegistedPlannerActive(InPlanner);

	if (Cast<USP_AIPlannerComponent>(InPlanner))
		OnAINumChange.Broadcast(++ActiveAINum);
}
void ASP_AIDirector::OnRegistedPlannerInactive(USP_PlannerComponent* InPlanner)
{
	Super::OnRegistedPlannerInactive(InPlanner);

	if (Cast<USP_AIPlannerComponent>(InPlanner))
		OnAINumChange.Broadcast(--ActiveAINum);
}

void ASP_AIDirector::RegisterFormationSet(USP_FormationSet* InFormationSet)
{
	ASP_AIDirector* AIInstance = Cast<ASP_AIDirector>(Instance);
	SP_SCHECK(AIInstance, "AI Instance nullptr! AIDirector actor must be put in scene.")

	AIInstance->RegisterFormationSet_Internal(InFormationSet);
}
void ASP_AIDirector::UnRegisterFormationSet(USP_FormationSet* InFormationSet)
{
	ASP_AIDirector* AIInstance = Cast<ASP_AIDirector>(Instance);
	SP_SCHECK(AIInstance, "AI Instance nullptr! AIDirector actor must be put in scene.")

	AIInstance->UnRegisterFormationSet_Internal(InFormationSet);
}

void ASP_AIDirector::RegisterFormationSet_Internal_Implementation(USP_FormationSet* InFormationSet)
{
	SP_CHECK_NULLPTR(InFormationSet)
	SP_CHECK(FormationSets.Find(InFormationSet) == INDEX_NONE, "FormationSet [%s] already registered!", *InFormationSet->GetName())

	FormationSets.Add(InFormationSet);
	OnFormationSetRegistered.Broadcast(InFormationSet);
}
void ASP_AIDirector::UnRegisterFormationSet_Internal_Implementation(USP_FormationSet* InFormationSet)
{
	SP_CHECK_NULLPTR(InFormationSet)
	SP_CHECK(FormationSets.Find(InFormationSet) != INDEX_NONE, "FormationSet [%s] not previously registered!", *InFormationSet->GetName())

	FormationSets.Remove(InFormationSet);
	OnFormationSetUnRegistered.Broadcast(InFormationSet);
}

void ASP_AIDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (int i = 0; i < FormationSets.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(FormationSets[i])
		FormationSets[i]->Reset();
	}

	// Reset Instance to nullptr.
	Super::EndPlay(EndPlayReason);
}
void ASP_AIDirector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update registered formations.
	for (int i = 0; i < FormationSets.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(FormationSets[i])
		FormationSets[i]->Tick(DeltaSeconds);
	}
}