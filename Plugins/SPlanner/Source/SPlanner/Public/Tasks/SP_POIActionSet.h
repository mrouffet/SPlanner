#pragma once

#include <Tasks/SP_POIAction.h>

#include <Engine/DataAsset.h>
#include "SP_POIActionSet.generated.h"

/**
 *	Set of Planner POI Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_POIActionSet : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** List of available actions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_POIAction> Actions;

public:
	/** Getter of Actions. */
	const TArray<FSP_POIAction>& GetActions() const;
};