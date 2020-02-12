// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/AI/POI/SP_POIComponent.h>

USP_POIComponent::USP_POIComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SphereRadius = 50.0f;

	// Set default POI tag.
	ComponentTags.Add("SP_POI");
}

USP_POIActionSet* USP_POIComponent::GetActionSet() const
{
	return ActionsSet;
}