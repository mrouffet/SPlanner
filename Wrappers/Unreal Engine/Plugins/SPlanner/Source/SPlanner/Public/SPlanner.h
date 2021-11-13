// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(SPlanner, Log, All);

namespace SP
{
	class Log;
}

class FSPlannerModule : public IModuleInterface
{
	static void API_LogCallback(const SP::Log& _log);

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
