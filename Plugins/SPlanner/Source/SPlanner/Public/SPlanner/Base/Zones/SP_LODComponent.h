#pragma once

#include <SPlanner/Base/Zones/SP_ZoneComponent.h>
#include "SP_LODComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSP_LODDelegate);

/**
*	SPlanner LOD base behavior.
*	Use radius and collision with players.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, DisplayName = "SP_LODComponent", ClassGroup = "SPlanner|Zone|LOD")
class SPLANNER_API USP_LODComponent : public USP_ZoneComponent
{
	GENERATED_BODY()

	void AddObject_Implementation(UPrimitiveComponent* Object) override;
	void RemoveObject_Implementation(UPrimitiveComponent* Object) override;

public:
	/** Callback events trigger when first object enter zone. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Zone|LOD")
	FSP_LODDelegate OnActive;

	/** Callback events trigger when last object exit zone. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Zone|LOD")
	FSP_LODDelegate OnInactive;

	USP_LODComponent(const FObjectInitializer& ObjectInitializer);

	/** Whether Objects with Tags are in range. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Zone|LOD")
	bool IsInRange() const;

	/**
	*	Get the level of LOD.
	*	Min distance between this and the closest object in Objects.
	*/
	UFUNCTION(BlueprintPure, Category = "SPlanner|Zone|LOD")
	float GetLevel() const;
};