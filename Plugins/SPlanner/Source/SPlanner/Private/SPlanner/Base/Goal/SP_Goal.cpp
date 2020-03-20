// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Director/SP_Director.h>
#include <SPlanner/Base/Planner/SP_PlannerComponent.h>

bool USP_Goal::GetResetBlackboard() const
{
	return bResetBlackboard;
}

const TArray<USP_PlannerComponent*>& USP_Goal::GetPlanners() const
{
	return Planners;
}

bool USP_Goal::CanStart_Implementation(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return true;
}
bool USP_Goal::CanLeave_Implementation(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return true;
}

void USP_Goal::OnStart_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(Planners.Find(InPlanner) == INDEX_NONE, "Planner [%s] already registered!", *InPlanner->GetOwner()->GetName())

	Planners.Add(InPlanner);

	if (Planners.Num() == 1)
		ASP_Director::RegisterGoal(this);
}
void USP_Goal::OnEnd_Implementation(USP_PlannerComponent* InPlanner)
{
	SP_CHECK_NULLPTR(InPlanner)
	SP_CHECK(Planners.Find(InPlanner) != INDEX_NONE, "Planner [%s] not previously registered!", *InPlanner->GetOwner()->GetName())

	Planners.Remove(InPlanner);

	if (Planners.Num() == 0)
		ASP_Director::UnRegisterGoal(this);
}

void USP_Goal::Reset_Implementation()
{
	Planners.Empty();
}

bool USP_Goal::CanTransitTo(const USP_PlannerComponent* Planner, const USP_Goal* OldGoal, const USP_Goal* NewGoal, bool bForce)
{
	SP_SRCHECK_NULLPTR(Planner, false)

	// From nullptr to NewGoal.
	if(!OldGoal)
	{
		SP_SRCHECK(NewGoal, false, "OldGoal and NewGoal are nullptr!")
		return NewGoal->CanStart(Planner);
	}

	// Froam OldGoal to nullptr.
	if (!NewGoal)
		return OldGoal->CanLeave(Planner);


	if (!OldGoal->CanLeave(Planner))
		return false;

	// Force doesn't check transitions.
	if(bForce)
		return NewGoal->CanStart(Planner);

	// Check OUT Transition.
	SP_SRCHECK(OldGoal->AllowedOutTransitions.Find(NewGoal) == INDEX_NONE || OldGoal->BlockedOutTransitions.Find(NewGoal) == INDEX_NONE,
	false, "Goal [%s] is both allowed and blocked in [%s]!", *NewGoal->GetName(), *OldGoal->GetName())

	// Allowed transition is empty: accept all non-blocked goals.
	if (!OldGoal->AllowedOutTransitions.Num())
	{
		// Found in blocked.
		if (OldGoal->BlockedOutTransitions.Find(NewGoal) != INDEX_NONE)
			return false;
	}
	else if (OldGoal->AllowedOutTransitions.Find(NewGoal) == INDEX_NONE)
		return false; // Not found in allowed.


	// Check IN Transition.
	SP_SRCHECK(NewGoal->AllowedInTransitions.Find(OldGoal) == INDEX_NONE || NewGoal->BlockedInTransitions.Find(OldGoal) == INDEX_NONE,
	false, "Goal [%s] is both allowed and blocked in [%s]!", *OldGoal->GetName(), *NewGoal->GetName())

	// Allowed transition is empty: accept all non-blocked goals.
	if (!NewGoal->AllowedInTransitions.Num())
	{
		// Found in blocked.
		if (NewGoal->BlockedInTransitions.Find(OldGoal) != INDEX_NONE)
			return false;
	}
	else if (NewGoal->AllowedInTransitions.Find(OldGoal) == INDEX_NONE)
		return false; // Not found in allowed.

	return NewGoal->CanStart(Planner);
}

#if SP_DEBUG_EDITOR
void USP_Goal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "AllowedInTransitions" || PropertyChangedEvent.Property->GetName() == "BlockedInTransitions")
	{
		if (AllowedInTransitions.Num() && BlockedInTransitions.Num())
		{
			for (int i = 0; i < AllowedInTransitions.Num(); ++i)
			{
				if (AllowedInTransitions[i] && BlockedInTransitions.Find(AllowedInTransitions[i]) != INDEX_NONE)
					SP_LOG_SCREEN(Error, FColor::Red, "Goal [%s] is both allowed and blocked!", *AllowedInTransitions[i]->GetName())
			}
		}
	}

	if (PropertyChangedEvent.Property->GetName() == "AllowedOutTransitions" || PropertyChangedEvent.Property->GetName() == "BlockedOutTransitions")
	{
		if (AllowedOutTransitions.Num() && BlockedOutTransitions.Num())
		{
			for (int i = 0; i < AllowedOutTransitions.Num(); ++i)
			{
				if (AllowedOutTransitions[i] && BlockedOutTransitions.Find(AllowedOutTransitions[i]) != INDEX_NONE)
					SP_LOG_SCREEN(Error, FColor::Red, "Goal [%s] is both allowed and blocked!", *AllowedOutTransitions[i]->GetName())
			}
		}
	}
}
#endif