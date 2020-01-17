#pragma once

#include <Components/SphereComponent.h>
#include "SP_POIComponent.generated.h"

class USP_POIActionSet;

/**
*	Planner POI behavior.
*	Grant special tasks to planner.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "PointOfInterest", ClassGroup = "SPlanner|POI")
class SPLANNER_API USP_POIComponent : public USphereComponent
{
	GENERATED_BODY()

protected:
	/** Action sets granted by this POI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_POIActionSet* ActionsSet;

public:
	USP_POIComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of ActionsSet. */
	USP_POIActionSet* GetActionSet() const;
};