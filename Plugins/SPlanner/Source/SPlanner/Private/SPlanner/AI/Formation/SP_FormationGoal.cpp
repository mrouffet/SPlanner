// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>
#include <SPlanner/AI/Formation/SP_FormationSet.h>

void USP_FormationGoal::OnStart_Implementation(USP_PlannerComponent* Planner)
{
	Super::OnStart_Implementation(Planner);

	USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_CHECK(AIPlanner, "AIPlanner is nullptr! Planner must be a USP_AIPlannerComponent.")

	SP_CHECK_NULLPTR(FormationSet)
	FormationSet->Add(AIPlanner);
}
void USP_FormationGoal::OnEnd_Implementation(USP_PlannerComponent* Planner)
{
	Super::OnEnd_Implementation(Planner);

	USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_CHECK(AIPlanner, "AIPlanner is nullptr! Planner must be a USP_AIPlannerComponent.")

	SP_CHECK_NULLPTR(FormationSet)
	FormationSet->Remove(AIPlanner);
}