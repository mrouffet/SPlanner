// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#include <Base/AI/SP_AssetTypeActions_AIAsset.h>

#include "AIModule.h"

#define LOCTEXT_NAMESPACE "SP_AssetTypeActions_AIAsset"

uint32 FSP_AssetTypeActions_AIAsset::GetCategories()
{
	IAIModule& AIModule = FModuleManager::LoadModuleChecked<IAIModule>("AIModule").Get();
	return AIModule.GetAIAssetCategoryBit();
}

#undef LOCTEXT_NAMESPACE
