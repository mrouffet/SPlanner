#pragma once

#include <SPlanner/Components/Zones/SP_ZoneComponent.h>
#include "SP_LODComponent.generated.h"

/**
*	SPlanner LOD base behavior.
*	Use radius and collision with players.
*/
UCLASS(Abstract, Blueprintable, BlueprintType, DisplayName = "LODComponent", ClassGroup = "SPlanner|LOD", meta=(BlueprintSpawnableComponent))
class SPLANNER_API USP_LODComponent : public USP_ZoneComponent
{
	GENERATED_BODY()

public:
	USP_LODComponent(const FObjectInitializer& ObjectInitializer);

	/** Whether Objects with Tags are in range. */
	bool IsInRange() const;

	/** Get the closest distance between this and the closest object in Objects. */
	float GetClosestDistance() const;
};