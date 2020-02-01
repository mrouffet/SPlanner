// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Formation/SP_FormationGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/AI/Formation/SP_FormationSet.h>
#include <SPlanner/AI/Planner/SP_AIPlannerComponent.h>

bool USP_FormationGoal::CanStart_Implementation(USP_PlannerComponent* Planner) const
{
	if (!Super::CanStart_Implementation(Planner))
		return false;

	SP_RCHECK_NULLPTR(FormationSet, false)

	USP_AIPlannerComponent* AIPlanner = Cast<USP_AIPlannerComponent>(Planner);

	SP_RCHECK(AIPlanner, false, "AIPlanner nullptr! Planner must be of type USP_AIPlannerComponent!")

	return FormationSet->CanAdd(AIPlanner);
}
bool USP_FormationGoal::CanLeave_Implementation(USP_PlannerComponent* Planner) const
{
	if (!Super::CanLeave_Implementation(Planner))
		return false;

	SP_RCHECK_NULLPTR(FormationSet, false)

	USP_AIPlannerComponent* AIPlanner = Cast<USP_AIPlannerComponent>(Planner);

	SP_RCHECK(AIPlanner, false, "AIPlanner nullptr! Planner must be of type USP_AIPlannerComponent!")

	return FormationSet->CanRemove(AIPlanner);
}

void USP_FormationGoal::OnStart_Implementation(USP_PlannerComponent* Planner)
{
	Super::OnStart_Implementation(Planner);

	USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_CHECK(AIPlanner, "AIPlanner is nullptr! Planner must be a USP_AIPlannerComponent.")

	SP_CHECK_NULLPTR(FormationSet)

	if (!FormationSet->Add(AIPlanner))
	{
		SP_LOG(Error, "Can't add [%s] in formation!", *AIPlanner->GetOwner()->GetName())
		return;
	}
}
void USP_FormationGoal::OnEnd_Implementation(USP_PlannerComponent* Planner)
{
	Super::OnEnd_Implementation(Planner);

	USP_AIPlannerComponent* const AIPlanner = Cast<USP_AIPlannerComponent>(Planner);
	SP_CHECK(AIPlanner, "AIPlanner is nullptr! Planner must be a USP_AIPlannerComponent.")

	SP_CHECK_NULLPTR(FormationSet)

	if (!FormationSet->Remove(AIPlanner))
	{
		SP_LOG(Error, "Can't remove [%s] in formation!", *AIPlanner->GetOwner()->GetName())
		return;
	}
}