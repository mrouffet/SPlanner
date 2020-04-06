// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_Actor.h>
#include "SP_POIActor.generated.h"

class USP_POIComponent;
class USP_POIActionSet;

UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|POI")
class SPLANNER_API ASP_POIActor : public ASP_Actor
{
	GENERATED_BODY()

protected:
	/** POI behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner|POI")
	USP_POIComponent* POI = nullptr;

public:
	ASP_POIActor(const FObjectInitializer& ObjectInitializer);

	/** Getter of PointOfInterest->ActionSet. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|POI")
	USP_POIActionSet* GetActionSet() const;
};