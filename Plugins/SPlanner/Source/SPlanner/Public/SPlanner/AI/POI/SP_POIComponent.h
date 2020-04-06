// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_ActorComponent.h>
#include "SP_POIComponent.generated.h"

class USP_POIActionSet;

/**
*	Planner POI behavior.
*	Grant special tasks to planner.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "SP_POI", ClassGroup = "SPlanner|POI")
class SPLANNER_API USP_POIComponent : public USP_ActorComponent
{
	GENERATED_BODY()

protected:
	/** Action sets granted by this POI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_POIActionSet* ActionsSet;

public:
	/** Getter of ActionsSet. */
	USP_POIActionSet* GetActionSet() const;
};