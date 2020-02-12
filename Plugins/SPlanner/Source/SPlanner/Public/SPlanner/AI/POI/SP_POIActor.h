#pragma once

#include <GameFramework/Actor.h>
#include "SP_POIActor.generated.h"

class USP_POIComponent;
class USP_POIActionSet;

UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|POI")
class SPLANNER_API ASP_POIActor : public AActor
{
	GENERATED_BODY()

protected:
	/** POI behavior. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_POIComponent* POI = nullptr;

public:
	ASP_POIActor(const FObjectInitializer& ObjectInitializer);

	/** Getter of PointOfInterest->ActionSet. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|POI")
	USP_POIActionSet* GetActionSet() const;
};