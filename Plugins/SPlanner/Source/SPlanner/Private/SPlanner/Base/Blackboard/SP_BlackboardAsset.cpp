// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Base/Blackboard/SP_BlackboardAsset.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Base/Goal/SP_Goal.h>

USP_ActionSet* USP_BlackboardAsset::GetActionSet(USP_Goal* Goal)
{
	USP_ActionSet* const* const ActionSetPtr = ActionsSets.Find(Goal);

	SP_RCHECK(ActionSetPtr && *ActionSetPtr, nullptr, "No action set for goal [ %s ]", *Goal->GetName())

	return *ActionSetPtr;
}