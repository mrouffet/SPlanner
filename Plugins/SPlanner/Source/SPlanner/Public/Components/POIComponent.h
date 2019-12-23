#pragma once

#include <Components/ActorComponent.h>
#include "POIComponent.generated.h"

class USP_POIActionSet;

/**
*	Planner POI behavior.
*	Grant special tasks to planner.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "PointOfInterest", ClassGroup = "SPlanner|POI")
class SPLANNER_API USP_POIComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Action sets granted by this POI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|POI")
	USP_POIActionSet* ActionsSet;

public:
	/** Getter of ActionsSet. */
	USP_POIActionSet* GetActionSet() const;
};