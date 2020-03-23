// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/SP_AIBlackboardEntry.h>

#include <SPlanner/Base/Blackboard/SP_BlackboardAsset.h>
#include "SP_AIBlackboardAsset.generated.h"

/**
 *	SPlanner AI Blackboard asset implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard|AI")
class SPLANNER_API USP_AIBlackboardAsset : public USP_BlackboardAsset
{
	GENERATED_BODY()

protected:
	/** List of blackboard entries to store data. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|AI")
	TArray<FSP_AIBlackboardEntry> Entries;

public:
	USP_AIBlackboardAsset(const FObjectInitializer& ObjectInitializer);

	/** Getter of Entries. */
	const TArray<FSP_AIBlackboardEntry>& GetEntries() const;
};