// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Goal/SP_AIGoal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Misc/VariableAsset/SP_IntAsset.h>
#include <SPlanner/Misc/VariableAsset/SP_FloatAsset.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>
#include <SPlanner/AI/Blackboard/SP_AIBlackboardComponent.h>

float USP_AIGoal::GetMinPlannerNum() const
{
	return MinPlannerNum;
}
float USP_AIGoal::GetMaxPlannerNum() const
{
	return MaxPlannerNum;
}

bool USP_AIGoal::CanStart_Implementation(const USP_PlannerComponent* Planner) const
{
	if (!Super::CanStart_Implementation(Planner))
		return false;

	// Maximum planners reached.
	if (MaxPlannerNum > 0 && Planners.Num() >= MaxPlannerNum)
		return false;

	return true;
}
bool USP_AIGoal::CanEnd_Implementation(const USP_PlannerComponent* Planner) const
{
	if (!Super::CanEnd_Implementation(Planner))
		return false;

	// Minimum planners reached.
	if (MinPlannerNum > 0 && Planners.Num() <= MinPlannerNum)
		return false;

	return true;
}

void USP_AIGoal::OnStart_Implementation(USP_PlannerComponent* InPlanner)
{
	Super::OnStart_Implementation(InPlanner);

	if (OutputPlannerNum)
		OutputPlannerNum->Set(Planners.Num());

	if (OutputMaxRatio && MaxPlannerNum > 0)
		OutputMaxRatio->Set(static_cast<float>(Planners.Num()) / MaxPlannerNum);

	// Blackboard must be reset to start this goal.
	if (bResetBlackboard)
	{
		if (USP_AIBlackboardComponent* const AIBlackboard = InPlanner->GetBlackboard<USP_AIBlackboardComponent>())
			AIBlackboard->Reset();
	}
}
void USP_AIGoal::OnEnd_Implementation(USP_PlannerComponent* InPlanner)
{
	Super::OnEnd_Implementation(InPlanner);

	if (OutputPlannerNum)
		OutputPlannerNum->Set(Planners.Num());

	if (OutputMaxRatio && MaxPlannerNum > 0)
		OutputMaxRatio->Set(static_cast<float>(Planners.Num()) / MaxPlannerNum);
}