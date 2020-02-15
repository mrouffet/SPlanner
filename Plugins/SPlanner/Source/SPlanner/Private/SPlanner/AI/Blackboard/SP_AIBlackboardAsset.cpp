// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/BlackBoard/SP_AIBlackboardAsset.h>

const TArray<FSP_AIBlackboardEntry>& USP_AIBlackboardAsset::GetEntries() const
{
	return Entries;
}