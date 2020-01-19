#pragma once

#include <SPlanner/Components/Zones/SP_ZoneComponent.h>
#include "SP_POIZoneComponent.generated.h"

class USP_POIComponent;

/**
 *	Implementation of planner interact zone.
 *	Used to pickup POI action set.
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "POIZone", ClassGroup = "SPlanner|POIZone", meta = (BlueprintSpawnableComponent))
class SPLANNER_API USP_POIZoneComponent : public USP_ZoneComponent
{
	GENERATED_BODY()
	
protected:
	void AddObject_Implementation(UPrimitiveComponent* Object) override;

public:
	USP_POIZoneComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of POIs. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|POIZone")
	const TArray<USP_POIComponent*>& GetPOIs() const;
};