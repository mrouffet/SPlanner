// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/SPlanner.h>

#include <SPlanner/Debug/SP_DebugBenchmark.h>

#define LOCTEXT_NAMESPACE "FSPlannerModule"

void FSPlannerModule::StartupModule()
{
	SP_BENCHMARK_SCOPE(Startup)
}

void FSPlannerModule::ShutdownModule()
{
	SP_BENCHMARK_SCOPE(Shutdown)
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSPlannerModule, SPlanner)