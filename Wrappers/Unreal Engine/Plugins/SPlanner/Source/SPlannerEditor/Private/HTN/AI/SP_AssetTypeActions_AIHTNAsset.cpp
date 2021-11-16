// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <HTN/AI/SP_AssetTypeActions_AIHTNAsset.h>

#include <HTN/AI/SP_AIHTNAsset.h>

#define LOCTEXT_NAMESPACE "SP_AssetTypeActions_AIHTNAsset"

FText FSP_AssetTypeActions_AIHTNAsset::GetName() const
{
	return NSLOCTEXT("SP_AssetTypeActions_AIHTNAsset", "SP_AssetTypeActions_AIHTNAsset", "HTN SPlanner");
}

FColor FSP_AssetTypeActions_AIHTNAsset::GetTypeColor() const
{
	return FColor(196, 96, 18);
}

UClass* FSP_AssetTypeActions_AIHTNAsset::GetSupportedClass() const
{
	return USP_AIHTNAsset::StaticClass();
}

#undef LOCTEXT_NAMESPACE
