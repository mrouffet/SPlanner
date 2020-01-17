#include <SPlanner/Actions/SP_PlannerActionSet.h>

struct DefaultPredicate
{
	bool operator()(const FSP_Action& Action) const
	{
		return true;
	}
};

FSP_PlannerActionSet::FSP_PlannerActionSet(const TArray<FSP_Action>& InBeginActions, const TArray<FSP_Action>& InCoreActions,
	const TArray<FSP_Action>& InForcedCoreActions, const TArray<FSP_Action>& InEndActions) :
	FSP_PlannerActionSet(InBeginActions, InCoreActions, InForcedCoreActions, InEndActions, DefaultPredicate())
{
}