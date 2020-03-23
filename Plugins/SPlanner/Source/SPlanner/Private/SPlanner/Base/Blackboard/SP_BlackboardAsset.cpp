// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Blackboard/SP_BlackboardAsset.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

#include <SPlanner/Base/Action/SP_ActionSet.h>

USP_BlackboardAsset::USP_BlackboardAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITOR
	ActionSetClass = USP_ActionSet::StaticClass();
#endif
}

USP_ActionSet* USP_BlackboardAsset::GetActionSet(USP_Goal* Goal)
{
	USP_ActionSet* const* const ActionSetPtr = ActionsSets.Find(Goal);

	SP_RCHECK(ActionSetPtr && *ActionSetPtr, nullptr, "No action set for goal [ %s ]", *Goal->GetName())

	return *ActionSetPtr;
}

#if WITH_EDITOR
void USP_BlackboardAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SP_CHECK(ActionSetClass.Get(), "Invalid ActionSetClass")

	// Check ActionSet class.
	if (PropertyChangedEvent.GetPropertyName() == "ActionsSets")
	{
		for (auto it = ActionsSets.begin(); it != ActionsSets.end(); ++it)
		{
			if (it->Value && !it->Value->GetClass()->IsChildOf(ActionSetClass.Get()))
			{
				SP_LOG(Error, "ActionSetClass[%s] is not a %s!", *(it->Key ? it->Key->GetName() : "None"), *ActionSetClass.Get()->GetName())

				it->Value = nullptr;
			}
		}
	}
}
#endif