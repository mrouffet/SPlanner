// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Decorator/SP_AIPlannerDecorator.h>
#include "SP_CheckBlackboardValueAIDecorator.generated.h"

class USP_AIBlackboardKey;

/**
 *	SPlanner decorator check blackboard entry is visible from pawn.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_CheckBlackboardValue", ClassGroup = "SPlanner|Decorator|AI")
class SPLANNER_API USP_CheckBlackboardValueAIDecorator : public USP_AIPlannerDecorator
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FName EntryName = "None";

	/** The value to compare with. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_AIBlackboardKey* KeyValue = nullptr;

	/** Internal implementation of Validate. */
	bool Validate_Internal_Implementation(const UObject* Object) override;
};