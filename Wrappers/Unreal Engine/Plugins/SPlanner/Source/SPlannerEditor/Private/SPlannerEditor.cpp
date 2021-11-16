// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPlannerEditor.h"

#include <Styling/SlateStyleRegistry.h>

#include <GOAP/AI/SP_AssetTypeActions_AIGOAPAsset.h>
#include <GOAP/AI/SP_AIGOAPAssetFactory.h>

#include <HTN/AI/SP_AssetTypeActions_AIHTNAsset.h>
#include <HTN/AI/SP_AIHTNAssetFactory.h>

DEFINE_LOG_CATEGORY(SPlannerEditor);

#define LOCTEXT_NAMESPACE "FSPlannerEditorModule"

void FSPlannerEditorModule::StartupModule()
{
	// Register asset types
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FSP_AssetTypeActions_AIGOAPAsset));
		FSlateStyleRegistry::RegisterSlateStyle(*USP_AIGOAPAssetFactory::StyleSet.Get());

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FSP_AssetTypeActions_AIHTNAsset));
		FSlateStyleRegistry::RegisterSlateStyle(*USP_AIHTNAssetFactory::StyleSet.Get());
	}
}

void FSPlannerEditorModule::ShutdownModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*USP_AIGOAPAssetFactory::StyleSet.Get());
	FSlateStyleRegistry::UnRegisterSlateStyle(*USP_AIHTNAssetFactory::StyleSet.Get());


	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		// Unregister our custom created assets from the AssetTools
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
	}

	CreatedAssetTypeActions.Empty();
}


void FSPlannerEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSPlannerEditorModule, SPlannerEditor)
