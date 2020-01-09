#include <Tasks/SP_Action.h>

FSP_Action::FSP_Action(USP_Task* InTask, float InWeight, bool InbAchieveGoal) :
	Task{ InTask },
	Weight{ InWeight },
	bAchieveGoal{ InbAchieveGoal }
{
}
FSP_Action::FSP_Action(const FSP_Action& Other, float WeightMultiplier) : FSP_Action(Other.Task, Other.Weight * WeightMultiplier, Other.bAchieveGoal)
{
}