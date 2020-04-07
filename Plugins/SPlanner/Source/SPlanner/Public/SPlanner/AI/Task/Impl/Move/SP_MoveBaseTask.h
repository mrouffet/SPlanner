// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_MoveBaseTask.generated.h"

/**
 *	MoveTo implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Move")
class SPLANNER_API USP_MoveBaseTask : public USP_TaskImpl
{
	GENERATED_BODY()
	
protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	FName TargetEntryName = "MainTarget";

	
	/** Can move to again even if a previous move to has been set in plan. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	bool bAllowMultipleMove = true;

	/**
	*	The pawn speed to set.
	*	Set < 0.0f to disable.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Move")
	float PawnSpeed = -1.0f;

	/** Child implementation to get pawn speed. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|Move")
	float GetPawnSpeed(APawn* Pawn);

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
};