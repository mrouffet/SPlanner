#include <SPlanner/Base/Zones/SP_ReactZoneComponent.h>

#include <TimerManager.h>

#include <SPlanner/Debug/SP_Debug.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

#if SP_DEBUG
	#include <GameFramework/Character.h>
	#include <GameFramework/PlayerController.h>
#endif

USP_ReactZoneComponent::USP_ReactZoneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Default active radius.
	SphereRadius = 400.0f;

	// Add default player tag.
	Tags.Add("SP_Player");
}

void USP_ReactZoneComponent::SetPlanner(USP_PlannerComponent* InPlanner)
{
	Planner = InPlanner;

	// Planner get reset.
	if (!Planner || Objects.Num() <= 0)
		return;
	
	// Save prev goal.
	if (bSavePreviousGoal)
		ExitGoal = Planner->GetGoal();

	ReactOnAdd();
}

void USP_ReactZoneComponent::ReactOnAdd_Implementation()
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK(GoalPairs.Num(), "No goal pairs!")

	for (int i = 0; i < GoalPairs.Num(); ++i)
	{
		// Find Goal to set from object num.
		if (Objects.Num() <= GoalPairs[i].ObjectNum)
		{
			// Was not previously set.
			if (Objects.Num() - 1 < GoalPairs[i].ObjectNum)
			{
				SP_CHECK_NULLPTR(GoalPairs[i].Goal)

				Planner->SetGoal(GoalPairs[i].Goal);
			}

			return;
		}
	}
}

void USP_ReactZoneComponent::ReactOnRemove_Implementation()
{
	SP_CHECK_NULLPTR(Planner)
	SP_CHECK(GoalPairs.Num(), "No goal pairs!")

	// Exit.
	if (Objects.Num() == 0)
	{
		Planner->SetGoal(ExitGoal);
		return;
	}

	for (int i = 0; i < GoalPairs.Num(); ++i)
	{
		// Find Goal to set from object num.
		if (Objects.Num() <= GoalPairs[i].ObjectNum)
		{
			// Was not previously set.
			if (Objects.Num() + 1 > GoalPairs[i].ObjectNum)
			{
				SP_CHECK_NULLPTR(GoalPairs[i].Goal)

				Planner->SetGoal(GoalPairs[i].Goal);
			}

			return;
		}
	}
}

void USP_ReactZoneComponent::AddObject_Implementation(UPrimitiveComponent* Object)
{
	Super::AddObject_Implementation(Object);

	// Overlap before setting planner: wait for SetPlanner().
	if (!Planner)
		return;

	// Save prev goal.
	if (Objects.Num() == 1 && bSavePreviousGoal)
		ExitGoal = Planner->GetGoal();

	ReactOnAdd();
}
void USP_ReactZoneComponent::RemoveObject_Implementation(UPrimitiveComponent* Object)
{
	Super::RemoveObject_Implementation(Object);

	// Overlap before setting planner: wait for SetPlanner().
	if (!Planner)
		return;

	ReactOnRemove();
}