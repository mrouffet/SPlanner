// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_DataAsset.h>
#include "SP_POIActionSet.generated.h"

class USP_POIAction;

/**
 *	Set of Planner POI Action.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action")
class SPLANNER_API USP_POIActionSet : public USP_DataAsset
{
	GENERATED_BODY()

protected:
	/** List of available actions. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_POIAction*> Actions;

public:
	/** Getter of Actions. */
	const TArray<USP_POIAction*>& GetActions() const;
};