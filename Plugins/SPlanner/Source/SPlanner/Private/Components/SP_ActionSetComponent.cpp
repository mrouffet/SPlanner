#include <Components/SP_ActionSetComponent.h>

#include <Debug/SP_Debug.h>

#include <Goal/SP_Goal.h>

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