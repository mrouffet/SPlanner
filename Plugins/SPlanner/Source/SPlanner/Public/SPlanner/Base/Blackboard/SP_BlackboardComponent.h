// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Components/ActorComponent.h>
#include "SP_BlackboardComponent.generated.h"

class USP_Goal;
class USP_ActionSet;

class USP_BlackboardAsset;

/**
 *	SPlanner base Blackboard implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard")
class SPLANNER_API USP_BlackboardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/**
	*	The blackboard asset to use.
	*	Instantiate a copy of this asset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_BlackboardAsset* BlackboardAsset = nullptr;

public:
	USP_BlackboardComponent(const FObjectInitializer& ObjectInitializer);

	/** Forward Getter of action set depending on Goal. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	USP_ActionSet* GetActionSet(USP_Goal* Goal);
};