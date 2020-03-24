// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Zone/SP_ToogleActiveZoneComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

void USP_ToogleActiveZoneComponent::Add_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(Planners.Find(InPlanner) == INDEX_NONE, "Planner [%s] already contained", *InPlanner->GetName())

	InPlanner->SetEnableBehavior(Objects.Num() ? !bDefaultActiveState : bDefaultActiveState);

	Planners.Add(InPlanner);
}
void USP_ToogleActiveZoneComponent::AddRange(const TArray<USP_PlannerComponent*>& InPlanners)
{
	for (int i = 0; i < InPlanners.Num(); ++i)
		Add(InPlanners[i]);
}

void USP_ToogleActiveZoneComponent::Remove_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(Planners.Find(InPlanner) != INDEX_NONE, "Planner [%s] not contained", *InPlanner->GetName())

	Planners.Remove(InPlanner);

	// Reset to default state.
	InPlanner->SetEnableBehavior(bDefaultActiveState);
}
void USP_ToogleActiveZoneComponent::RemoveRange(const TArray<USP_PlannerComponent*>& InPlanners)
{
	for (int i = 0; i < InPlanners.Num(); ++i)
		Remove(InPlanners[i]);
}

void USP_ToogleActiveZoneComponent::AddObject_Implementation(UPrimitiveComponent* Object)
{
	Super::AddObject_Implementation(Object);

	if (Objects.Num() == 1)
	{
		for (int i = 0; i < Planners.Num(); ++i)
		{
			SP_CCHECK(Planners[i], "Planners[%d] nullptr!", i)

			Planners[i]->SetEnableBehavior(!bDefaultActiveState);
		}
	}
}
void USP_ToogleActiveZoneComponent::RemoveObject_Implementation(UPrimitiveComponent* Object)
{
	Super::RemoveObject_Implementation(Object);

	if (Objects.Num() == 0)
	{
		for (int i = 0; i < Planners.Num(); ++i)
		{
			SP_CCHECK(Planners[i], "Planners[%d] nullptr!", i)

			Planners[i]->SetEnableBehavior(bDefaultActiveState);
		}
	}
}