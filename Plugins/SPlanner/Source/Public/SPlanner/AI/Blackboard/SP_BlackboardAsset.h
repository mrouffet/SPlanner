#pragma once

#include <SPlanner/AI/Blackboard/SP_BlackboardEntry.h>

#include <Engine/DataAsset.h>
#include "SP_BlackboardAsset.generated.h"

/**
 *	SPlanner Blackboard asset implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|AI|Blackboard")
class SPLANNER_API USP_BlackboardAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** List of blackboard entries to store data. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_BlackboardEntry> Entries;

public:
	/** Getter of Entries. */
	const TArray<FSP_BlackboardEntry>& GetEntries() const;
};