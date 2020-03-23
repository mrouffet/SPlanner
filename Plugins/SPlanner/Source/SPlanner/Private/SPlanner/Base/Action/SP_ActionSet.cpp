// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Action/SP_ActionSet.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Action/SP_Action.h>
#include <SPlanner/Base/Action/SP_PlannerActionSet.h>

USP_ActionSet::USP_ActionSet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActionClass = USP_Action::StaticClass();
}

const TArray<USP_Action*>& USP_ActionSet::GetBeginActions() const
{
	return BeginActions;
}
const TArray<USP_Action*>& USP_ActionSet::GetCoreActions() const
{
	return CoreActions;
}
const TArray<USP_Action*>& USP_ActionSet::GetForcedActions() const
{
	return ForcedActions;
}
const TArray<USP_Action*>& USP_ActionSet::GetEndActions() const
{
	return EndActions;
}

FSP_PlannerActionSet USP_ActionSet::Shuffle(const USP_PlannerComponent* Planner, bool* bCanBeAchievedPtr) const
{
	SP_RCHECK_NULLPTR(Planner, FSP_PlannerActionSet())
	SP_RCHECK(EndActions.Num(), FSP_PlannerActionSet(), "No EndActions: Can't generate a valid plan!")

	return FSP_PlannerActionSet::Make(Planner, this, bCanBeAchievedPtr);
}

#if WITH_EDITOR
void USP_ActionSet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "BeginActions")
		ForceActionClass(BeginActions);
	else if (PropertyChangedEvent.GetPropertyName() == "CoreActions")
		ForceActionClass(CoreActions);
	else if (PropertyChangedEvent.GetPropertyName() == "ForcedActions")
		ForceActionClass(ForcedActions);
	else if (PropertyChangedEvent.GetPropertyName() == "EndActions")
		ForceActionClass(EndActions);
}

void USP_ActionSet::ForceActionClass(TArray<USP_Action*>& Actions)
{
	for (int i = 0; i < Actions.Num(); ++i)
	{
		if (Actions[i] == nullptr || !Actions[i]->GetClass()->IsChildOf(ActionClass))
		{
			Actions[i] = NewObject<USP_Action>(this, ActionClass);
			return;
		}
	}
}
#endif