// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_LookAtTask.generated.h"

class USP_Target;

/**
 *	Look at task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task|Target")
class SPLANNER_API USP_LookAtTask : public USP_Task
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

	FRotator ComputeTargetRotation(const USP_AIPlannerComponent& Planner, const USP_Target* Target) const;

public:
	USP_TaskInfosBase* InstantiateInfos() override;

	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;
	uint64 PostCondition(const USP_PlannerComponent& Planner, uint64 PlannerFlags) const override;


	bool Begin(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos) override;
};


/** Task info implementation for USP_LookAtTask. */
UCLASS(ClassGroup = "SPlanner|Action|Task")
class USP_LookAtTaskInfos : public USP_TaskInfos
{
	GENERATED_BODY()

	// Only accessible by USP_LookAtTask.
	friend USP_LookAtTask;

	FRotator Start;
	FRotator End;

	float Alpha = 0.0f;
};