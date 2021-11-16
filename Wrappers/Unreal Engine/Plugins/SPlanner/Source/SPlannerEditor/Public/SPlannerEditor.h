// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(SPlannerEditor, Log, All);

#include <Modules/ModuleManager.h>

class FSPlannerEditorModule : public IModuleInterface
{
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
