// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/SP_POIActor.h>

#include <SPlanner/AI/POI/SP_POIComponent.h>

ASP_POIActor::ASP_POIActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	POI = CreateDefaultSubobject<USP_POIComponent>("SP_POI");
}

USP_POIActionSet* ASP_POIActor::GetActionSet() const
{
	return POI->GetActionSet();
}