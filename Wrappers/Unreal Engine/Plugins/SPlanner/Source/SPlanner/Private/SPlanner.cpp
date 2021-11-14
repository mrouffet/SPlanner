// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPlanner.h"

#include <SP/Core/Debug/Debug.hpp>

DEFINE_LOG_CATEGORY(SPlanner);

#define LOCTEXT_NAMESPACE "FSPlannerModule"

void FSPlannerModule::StartupModule()
{
	SP::logCallback = &FSPlannerModule::API_LogCallback;
}

void FSPlannerModule::ShutdownModule()
{
	SP::logCallback = nullptr;
}


void FSPlannerModule::API_LogCallback(const SP::Log& _log)
{
	const FString msg = (_log.function + " at " + _log.file + ":" + std::to_string(_log.line) + ":" + _log.message).c_str();

	switch (_log.level)
	{
		case SP::LogLevel::Warning:
		{
			UE_LOG(SPlanner, Warning, TEXT("%s"), *msg);
			break;
		}
		case SP::LogLevel::Error:
		{
			UE_LOG(SPlanner, Error, TEXT("%s"), *msg);
			break;
		}
		case SP::LogLevel::Normal:
		case SP::LogLevel::Infos:
		default:
		{
			UE_LOG(SPlanner, Display, TEXT("%s"), *msg);
			break;
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSPlannerModule, SPlanner)
