#include <SPlanner/Actions/SP_PlannerActionSet.h>

struct DefaultPredicate
{
	bool operator()(const FSP_Action& Action) const
	{
		return true;
	}
};

FSP_PlannerActionSet FSP_PlannerActionSet::Make(const USP_ActionSet* ActionSet, float LODLevel)
{
	return Make(ActionSet, LODLevel, DefaultPredicate());
}