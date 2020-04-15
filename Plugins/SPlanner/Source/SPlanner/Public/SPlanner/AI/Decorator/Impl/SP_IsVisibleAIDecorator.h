// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Decorator/SP_AIPlannerDecorator.h>
#include "SP_IsVisibleAIDecorator.generated.h"

/**
 *	SPlanner decorator check blackboard entry is visible from pawn.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_IsVisible", ClassGroup = "SPlanner|Decorator|AI")
class SPLANNER_API USP_IsVisibleAIDecorator : public USP_AIPlannerDecorator
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FName TargetEntryName = "None";

	/** Internal implementation of Validate. */
	bool Validate_Internal_Implementation(const UObject* Object) override;

	bool PreCondition_Validate_Internal_Implementation(const USP_PlanGenInfos* Infos) override;

public:
	USP_IsVisibleAIDecorator(const FObjectInitializer& ObjectInitializer);
};