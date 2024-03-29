// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Planner/SP_PlanState.h>

#include <SPlanner/AI/Task/SP_TaskMacro.h>
#include <SPlanner/AI/Task/SP_TaskNotification.h>
#include <SPlanner/AI/Planner/SP_AIPlannerNotify.h>

#include <SPlanner/Base/Action/SP_ActionImpl.h>
#include "SP_TaskImpl.generated.h"

class USP_TaskInfos;
class USP_AIPlannerComponent;

/**
 *	AI Planner task base.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_TaskImpl : public USP_ActionImpl
{
	GENERATED_BODY()

protected:
	/**
	*	The task infos class to instantiate.
	*	Value set in constructor in children implementations.
	*/
	TSubclassOf<USP_TaskInfos> TaskInfosClass;

	/**
	*	Maks of notify reaction.
	*	The task will either success or failed when a notify in mask will be throw, depending on bSuccessOnNotify.
	*/
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "ESP_AIPlannerNotify"), Category = "SPlanner|Task|Notify")
	uint8 NotifyMask = 0u;

	/** How the task should react to notifications. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Notify")
	ESP_TaskNotification NotifyAction = ESP_TaskNotification::TNA_TimeOut;

	/** Init the Notify callback and base execution state. */
	void InitNotify(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos);

	/**
	*	The begin internal implementation of the task.
	*	Called by Begin().
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Begin", Category = "SPlanner|Task")
	bool Begin_Internal(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos);

	/**
	*	The tick internal implementation of the task.
	*	Called by Tick().
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Tick", Category = "SPlanner|Task")
	ESP_PlanExecutionState Tick_Internal(float DeltaSeconds, USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos);

	/**
	*	The end implementation of the task.
	*	Called by End().
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "End", Category = "SPlanner|Task")
	bool End_Internal(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos);

	bool PreCondition_Implementation(const USP_PlanGenInfos* Infos) const override;
	bool PostCondition_Implementation(USP_PlanGenInfos* Infos) const override;
	bool ResetPostCondition_Implementation(USP_PlanGenInfos* Infos) const override;

public:
	USP_TaskImpl(const FObjectInitializer& ObjectInitializer);

	/** Freeze implementation of the task. */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Begin", Category = "SPlanner|Task")
	void Freeze(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos);

	/** UnFreeze implementation of the task. */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "Begin", Category = "SPlanner|Task")
	void UnFreeze(USP_AIPlannerComponent* Planner, USP_TaskInfos* TaskInfos);


	/** Callback method bound to planner. */
	UFUNCTION(Category = "SPlanner|Task")
	virtual void OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfos* TaskInfos);

	/** Instantiate the TaskInfos for this task. */
	USP_TaskInfos* InstantiateInfos(UObject* Outer = static_cast<UObject*>(GetTransientPackage()));

	/**
	*	The tick implementation of the task.
	*	Call Begin_Internal(), Tick_Internal() and End_Internal().
	*	This is executed by the PlannerComponent (main thread).
	*/
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos);

	/**
	*	Cancel the task.
	*	Call End_Internal().
	*/
	void Cancel(USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos);
};


/** Task info implementation for USP_TaskImpl. */
UCLASS(BlueprintType, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_TaskInfos : public UObject
{
	GENERATED_BODY()

	// Only accessible by USP_TaskImpl.
	friend USP_TaskImpl;

	ESP_PlanExecutionState BaseExecutionState = ESP_PlanExecutionState::PES_Succeed;

public:
	/** Whether the task has begun. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|Task")
	bool bHasBegun = false;

	/** Whether the task has been cancelled. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner|Task")
	bool bForcedEnd = false;

	/** Whether the task should bind to Notify events. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SPlanner|Task")
	bool bUseNotify = true;
};