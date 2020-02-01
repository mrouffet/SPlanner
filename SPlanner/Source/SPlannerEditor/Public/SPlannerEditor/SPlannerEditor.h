// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>

class FSPlannerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};