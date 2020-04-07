// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/POI/SP_POIActor.h>
#include "SP_AIPath.generated.h"

class USP_SplineComponent;

/**
*	SPlanner Path implementation using Spline.
*/
UCLASS(Blueprintable, BlueprintType, Category = "SPlanner|POI")
class SPLANNER_API ASP_AIPath : public ASP_POIActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner|POI|Path")
	USP_SplineComponent* Spline = nullptr;

public:
	ASP_AIPath(const FObjectInitializer& ObjectInitializer);

	/** Getter of Spline. */
	USP_SplineComponent* GetSpline() const;
};