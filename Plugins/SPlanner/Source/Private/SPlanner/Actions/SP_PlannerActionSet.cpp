#include <SPlanner/Actions/SP_PlannerActionSet.h>

struct DefaultPredicate
{
	bool operator()(const FSP_Action& Action) const
	{
		return true;
	}
};

const TArray<FSP_PlannerAction>& FSP_PlannerActionSet::GetActionsFromDepth(uint8 Depth) const
{
	if (Depth == 0 && BeginActions.Num())
		return BeginActions;

	return Actions;
}

FSP_PlannerActionSet FSP_PlannerActionSet::Make(const USP_ActionSet* ActionSet, float LODLevel)
{
	return Make(ActionSet, LODLevel, DefaultPredicate());
}