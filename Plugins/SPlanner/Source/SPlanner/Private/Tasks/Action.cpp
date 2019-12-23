#include <Tasks/Action.h>

FSP_Action::FSP_Action(USP_Task* InTask, float InWeight, bool InbAchieveGoal) :
	Task{ InTask },
	Weight{ InWeight },
	bAchieveGoal{ InbAchieveGoal }
{
}
FSP_Action::FSP_Action(const FSP_Action& Other, float WeightMultiplier) :
	Task{ Other.Task },
	Weight{ Other.Weight * WeightMultiplier },
	bAchieveGoal{ Other.bAchieveGoal }
{
}