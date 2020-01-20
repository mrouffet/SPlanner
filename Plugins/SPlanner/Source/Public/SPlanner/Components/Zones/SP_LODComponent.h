#pragma once

#include <SPlanner/Components/Zones/SP_ZoneComponent.h>
#include "SP_LODComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSP_LODDelegate);

/**
*	SPlanner LOD base behavior.
*	Use radius and collision with players.
*/
UCLASS(Abstract, Blueprintable, BlueprintType, DisplayName = "LODComponent", ClassGroup = "SPlanner|LOD", meta=(BlueprintSpawnableComponent))
class SPLANNER_API USP_LODComponent : public USP_ZoneComponent
{
	GENERATED_BODY()

	void AddObject_Implementation(UPrimitiveComponent* Object) override;
	void RemoveObject_Implementation(UPrimitiveComponent* Object) override;

public:
	/** Callback events trigger when first object enter zone. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|LOD")
	FSP_LODDelegate OnActive;

	/** Callback events trigger when last object exit zone. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|LOD")
	FSP_LODDelegate OnInactive;

	USP_LODComponent(const FObjectInitializer& ObjectInitializer);

	/** Whether Objects with Tags are in range. */
	bool IsInRange() const;

	/** Get the closest distance between this and the closest object in Objects. */
	float GetClosestDistance() const;
};