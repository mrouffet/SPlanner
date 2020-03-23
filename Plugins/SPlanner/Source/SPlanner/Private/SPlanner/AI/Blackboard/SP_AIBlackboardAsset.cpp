// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/BlackBoard/SP_AIBlackboardAsset.h>

#include <SPlanner/AI/Task/SP_TaskSet.h>

USP_AIBlackboardAsset::USP_AIBlackboardAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITOR
	ActionSetClass = USP_TaskSet::StaticClass();
#endif
}

const TArray<FSP_AIBlackboardEntry>& USP_AIBlackboardAsset::GetEntries() const
{
	return Entries;
}