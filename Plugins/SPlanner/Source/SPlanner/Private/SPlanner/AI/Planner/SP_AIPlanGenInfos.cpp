// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/Planner/SP_AIPlanGenInfos.h>

bool USP_AIPlanGenInfos::IsFlagSet(ESP_AIPlanGenFlags Flag) const
{
	return Flags.Find(Flag) != INDEX_NONE;
}

void USP_AIPlanGenInfos::AddFlag(ESP_AIPlanGenFlags Flag)
{
	Flags.Add(Flag);
}
void USP_AIPlanGenInfos::RemoveFlag(ESP_AIPlanGenFlags Flag)
{
	Flags.Remove(Flag);
}


bool USP_AIPlanGenInfos::IsBlackboardFlagSet(const FName& EntryName, ESP_AIBBPlanGenFlags Flag) const
{
	if (const TArray<ESP_AIBBPlanGenFlags>* const FlagsPtr = BlackboardFlags.Find(EntryName))
		return FlagsPtr->Find(Flag) != INDEX_NONE;

	return false;
}

void USP_AIPlanGenInfos::AddBlackboardFlag(const FName& EntryName, ESP_AIBBPlanGenFlags Flag)
{
	BlackboardFlags.FindOrAdd(EntryName).Add(Flag);
}
void USP_AIPlanGenInfos::RemoveBlackboardFlag(const FName& EntryName, ESP_AIBBPlanGenFlags Flag)
{
	if(TArray<ESP_AIBBPlanGenFlags>* const FlagsPtr = BlackboardFlags.Find(EntryName))
		FlagsPtr->Remove(Flag);
}