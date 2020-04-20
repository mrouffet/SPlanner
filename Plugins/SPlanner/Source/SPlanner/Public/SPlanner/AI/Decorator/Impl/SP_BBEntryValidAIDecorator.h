// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Decorator/SP_AIPlannerDecorator.h>
#include "SP_BBEntryValidAIDecorator.generated.h"

class USP_AIBlackboardKey;

/**
 *	SPlanner decorator check blackboard entry is valid.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BBEntryValid", ClassGroup = "SPlanner|Decorator|AI")
class SPLANNER_API USP_BBEntryValidAIDecorator : public USP_AIPlannerDecorator
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FName EntryName = "None";

	/** Internal implementation of Validate. */
	bool Validate_Internal_Implementation(const UObject* Object) override;
};