#include <SPlanner/Components/Zones/SP_POIZoneComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

#include <SPlanner/Components/SP_POIComponent.h>

USP_POIZoneComponent::USP_POIZoneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Add default POI tag.
	Tags.Add("SP_POI");
}

const TArray<USP_POIComponent*>& USP_POIZoneComponent::GetPOIs() const
{
	return reinterpret_cast<const TArray<USP_POIComponent*>&>(Objects);
}

void USP_POIZoneComponent::AddObject_Implementation(UPrimitiveComponent* Object)
{
	SP_CHECK(Cast<USP_POIComponent>(Object), "Object is not of type USP_POIComponent!")

	Super::AddObject_Implementation(Object);
}