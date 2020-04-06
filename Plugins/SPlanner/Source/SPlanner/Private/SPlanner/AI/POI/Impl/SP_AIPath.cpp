// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/Impl/SP_AIPath.h>

#include <SPlanner/Misc/SP_SplineComponent.h>

ASP_AIPath::ASP_AIPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Spline = CreateDefaultSubobject<USP_SplineComponent>("Spline");
	Spline->SetupAttachment(RootComponent);
}