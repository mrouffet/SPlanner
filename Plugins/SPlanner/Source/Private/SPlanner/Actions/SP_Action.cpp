#include <SPlanner/Actions/SP_Action.h>

FSP_Action::FSP_Action(USP_ActionStep* InStep, float InWeight) :
	FSP_ActionBase(InStep),
	Weight{ InWeight }
{
}