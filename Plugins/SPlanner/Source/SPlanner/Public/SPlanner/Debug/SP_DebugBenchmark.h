// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/SP_Config.h>

#if SP_DEBUG

#include <Engine/Engine.h>

DECLARE_STATS_GROUP(TEXT("SPlanner"), STATGROUP_SPlanner, STATCAT_Advanced);

#define SP_BENCHMARK_SCOPE(FunctionName) DECLARE_SCOPE_CYCLE_COUNTER(TEXT("SP -- " #FunctionName), STAT_##FunctionName, STATGROUP_SPlanner)

#else

#define SP_BENCHMARK_SCOPE(...)

#endif