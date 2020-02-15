// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Engine/DataAsset.h>
#include "SP_BlackboardAsset.generated.h"

class USP_Goal;
class USP_ActionSet;

/**
 *	SPlanner base Blackboard implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard")
class SPLANNER_API USP_BlackboardAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** Action sets depending on current goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TMap<USP_Goal*, USP_ActionSet*> ActionsSets;

public:
	/** Getter of action set depending on Goal. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	USP_ActionSet* GetActionSet(USP_Goal* Goal);
};