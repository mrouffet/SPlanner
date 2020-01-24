#include <SPlanner/Goal/SP_Goal.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <list>

/**
*	Breadth-first search goal in sub-goals.
*	https://en.wikipedia.org/wiki/Breadth-first_search
*/
class BFS_Goal
{
public:
	static bool IsSubGoal(const USP_Goal* ToFind, const USP_Goal* RootGoal)
	{
		const USP_Goal* ItGoal = RootGoal;
		std::list<const USP_Goal*> GoalList;

#if SP_DEBUG
		TArray<const USP_Goal*> VisitedGoals; // Check goal infinite loop.
#endif

		while (ItGoal)
		{
			if (ItGoal == ToFind)
				return true;

#if SP_DEBUG
			VisitedGoals.Add(ItGoal);
#endif

			for (int i = 0; i < ItGoal->GetSubGoals().Num(); ++i)
			{
				SP_SCCHECK(ItGoal->GetSubGoals()[i], "%s[%d] nullptr!", *ItGoal->GetName(), i)
				SP_SCCHECK(VisitedGoals.Find(ItGoal->GetSubGoals()[i]) == INDEX_NONE, "Inifite sub-goal loop: %s already visited!", *ItGoal->GetSubGoals()[i]->GetName())

				GoalList.push_back(ItGoal->GetSubGoals()[i]);
			}

			if (!GoalList.empty())
			{
				ItGoal = GoalList.front();
				GoalList.pop_front();
			}
			else
				ItGoal = nullptr;
		}

		return false;
	}
};

const TArray<USP_Goal*>& USP_Goal::GetSubGoals() const
{
	return SubGoals;
}

const TArray<USP_Goal*>& USP_Goal::GetAllowedTransitions() const
{
	return AllowedTransitions;
}
const TArray<USP_Goal*>& USP_Goal::GetBlockedTransitions() const
{
	return BlockedTransitions;
}

bool USP_Goal::CanTransitTo(USP_Goal* NewGoal) const
{
	// This is already a sub goal of NewGoal.
	if (BFS_Goal::IsSubGoal(this, NewGoal))
		return false;

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