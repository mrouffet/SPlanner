// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <GOAP/AI/SP_AssetTypeActions_AIGOAPAsset.h>

#include <GOAP/AI/SP_AIGOAPAsset.h>

#define LOCTEXT_NAMESPACE "SP_AssetTypeActions_AIGOAPAsset"

FText FSP_AssetTypeActions_AIGOAPAsset::GetName() const
{
	return NSLOCTEXT("SP_AssetTypeActions_AIGOAPAsset", "SP_AssetTypeActions_AIGOAPAsset", "GOAP SPlanner");
}

FColor FSP_AssetTypeActions_AIGOAPAsset::GetTypeColor() const
{
	return FColor(235, 186, 28);
}

UClass* FSP_AssetTypeActions_AIGOAPAsset::GetSupportedClass() const
{
	return USP_AIGOAPAsset::StaticClass();
}

#undef LOCTEXT_NAMESPACE
