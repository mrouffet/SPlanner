#include <SPlanner/Goals/SP_Goal.h>

#include <SPlanner/Debug/SP_Debug.h>

const TArray<const USP_Goal*>& USP_Goal::GetAllowedTransitions() const
{
	return AllowedTransitions;
}
const TArray<const USP_Goal*>& USP_Goal::GetBlockedTransitions() const
{
	return BlockedTransitions;
}

const USP_Goal* USP_Goal::GetExecutionGoal(const USP_PlannerComponent* Planner) const
{
	SP_RCHECK_NULLPTR(Planner, false)

	return this;
}

bool USP_Goal::CanTransitTo(const USP_Goal* NewGoal) const
{
	// Allowed transition is empty: accept all non-blocked goals.
	if (!AllowedTransitions.Num())
		return BlockedTransitions.Find(NewGoal) == INDEX_NONE;

	if (AllowedTransitions.Find(NewGoal) != INDEX_NONE)
	{
		SP_RCHECK(BlockedTransitions.Find(NewGoal) == INDEX_NONE, false, "Goal [%s] is both allowed and blocked!")
		return true;
	}

	return false;
}