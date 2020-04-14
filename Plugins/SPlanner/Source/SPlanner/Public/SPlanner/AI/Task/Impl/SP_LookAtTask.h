// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/SP_TaskImpl.h>
#include "SP_LookAtTask.generated.h"

class USP_Target;

/**
 *	Look at task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_LookAtTask : public USP_TaskImpl
{
	GENERATED_BODY()

protected:
	/** The entry name to access Target object in Blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	FName TargetEntryName = "None";

	/** Whether the rotation should be in one frame.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bInstant = false;

	/** Rotation speed.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target", meta=(EditCondition="!bInstant"))
	float Speed = 4.0f;

	/** Whether pitch is freeze.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bFreezePitch = true;

	/** Whether yaw is freeze.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bFreezeYaw = false;

	/** Whether roll is freeze.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Target")
	bool bFreezeRoll = true;

	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Task|Target")
	void ApplyRotation(APawn* Pawn, const FRotator& Rotation);

	FRotator ComputeTargetRotation(const USP_AIPlannerComponent* Planner, const USP_Target* Target) const;

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

	bool Begin_Internal_Implementation(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;
	ESP_PlanExecutionState Tick_Internal_Implementation(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos) override;

public:
	USP_LookAtTask(const FObjectInitializer& ObjectInitializer);
};


/** Task info implementation for USP_LookAtTask. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_LookAtTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_LookAtTask.
	friend USP_LookAtTask;

	FRotator Start;
	FRotator End;

	float Alpha = 0.0f;
};