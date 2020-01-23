#include <SPlanner/Components/SP_ActionSetComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Goal/SP_Goal.h>

USP_ActionSetComponent::USP_ActionSetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

USP_ActionSet* USP_ActionSetComponent::GetActionSet(USP_Goal* Goal)
{
	USP_ActionSet* const* const ActionSetPtr = ActionsSets.Find(Goal);

	SP_RCHECK(ActionSetPtr && *ActionSetPtr, nullptr, "No action set for goal [ %s ]", *Goal->GetName())

	return *ActionSetPtr;
}