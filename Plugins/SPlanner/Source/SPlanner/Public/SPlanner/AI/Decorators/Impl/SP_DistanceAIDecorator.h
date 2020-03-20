// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Decorators/SP_AIDecorator.h>
#include "SP_DistanceAIDecorator.generated.h"

/**
 *	SPlanner decorator check distance with blackboard entry.
 *	MinDistance < Distance Target-Pawn < MaxDistance
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator")
class SPLANNER_API USP_DistanceAIDecorator : public USP_AIDecorator
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FName TargetEntryName = "None";

	/**
	*	The minimum distance (near) to check with Target object.
	*	Use < 0.0f for no check.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float MinDistance = 10.0f;

	/**
	*	The maximum distance (far) to check with Target object.
	*	Use < 0.0f for no check.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float MaxDistance = 100.0f;

	/** Internal implementation of Validate. */
	bool Validate_Internal_Implementation(const USP_PlannerComponent* Planner) override;

	bool PreCondition_Validate_Internal_Implementation(const USP_PlannerComponent* Planner, const USP_PlanGenInfos* PlanGenInfos) override;
};