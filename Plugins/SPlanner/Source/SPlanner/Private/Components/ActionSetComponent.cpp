#include <Components/ActionSetComponent.h>

#include <Debug/Debug.h>

#include <Goal/Goal.h>

USP_ActionSetComponent::USP_ActionSetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

USP_ActionSet* USP_ActionSetComponent::GetActionSet(USP_Goal* Goal)
{
	USP_ActionSet* const* const ActionSetPtr = ActionsSets.Find(Goal);

	SP_RCHECK(ActionSetPtr && *ActionSetPtr, "No action set for goal [ %s ]", nullptr, *Goal->GetName())

	return *ActionSetPtr;
}