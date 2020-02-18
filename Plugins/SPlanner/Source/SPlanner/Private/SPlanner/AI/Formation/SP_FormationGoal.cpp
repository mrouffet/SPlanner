// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Formation/SP_FormationSet.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

bool USP_FormationGoal::OnStart_Implementation(USP_PlannerComponent* Planner)
{
	if (!Super::OnStart_Implementation(Planner))
		return false;

	USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_RCHECK(AIPlanner, false, "AIPlanner is nullptr! Planner must be a USP_AIPlannerComponent.")

	SP_RCHECK_NULLPTR(FormationSet, false)
	return FormationSet->AddSingle(AIPlanner);
}
bool USP_FormationGoal::OnEnd_Implementation(USP_PlannerComponent* Planner)
{
	if (!Super::OnEnd_Implementation(Planner))
		return false;

	USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_RCHECK(AIPlanner, false, "AIPlanner is nullptr! Planner must be a USP_AIPlannerComponent.")

	SP_RCHECK_NULLPTR(FormationSet, false)
	return FormationSet->RemoveSingle(AIPlanner);
}