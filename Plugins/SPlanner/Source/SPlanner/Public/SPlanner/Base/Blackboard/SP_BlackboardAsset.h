// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_DataAsset.h>
#include "SP_BlackboardAsset.generated.h"

class USP_Goal;
class USP_ActionSet;

/**
 *	SPlanner base Blackboard implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard")
class SPLANNER_API USP_BlackboardAsset : public USP_DataAsset
{
	GENERATED_BODY()

protected:
#if WITH_EDITOR
	TSubclassOf<USP_ActionSet> ActionSetClass = nullptr;
#endif

	/** Action sets depending on current goal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TMap<USP_Goal*, USP_ActionSet*> ActionsSets;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	USP_BlackboardAsset(const FObjectInitializer& ObjectInitializer);

	/** Getter of action set depending on Goal. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	USP_ActionSet* GetActionSet(USP_Goal* Goal);
};