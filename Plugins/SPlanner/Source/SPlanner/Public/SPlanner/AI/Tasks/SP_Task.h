// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <SPlanner/Base/Planner/SP_PlanState.h>

#include <SPlanner/AI/Tasks/SP_TaskMacro.h>
#include <SPlanner/AI/Tasks/SP_TaskInfosBase.h>
#include <SPlanner/AI/Tasks/SP_TaskNotification.h>
#include <SPlanner/AI/Planner/SP_AIPlannerNotify.h>

#include <SPlanner/Base/Actions/SP_ActionStep.h>
#include "SP_Task.generated.h"

class USP_AIPlannerComponent;

/**
 *	AI Planner task base.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_Task : public USP_ActionStep
{
	GENERATED_BODY()

protected:
	// Allow TaskChain to call OnNotify().
	friend class USP_TaskChain;

	/** The cooldown of this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Cooldown")
	FSP_FloatParam Cooldown;

	/** Should use cooldown even if the task has failed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Cooldown")
	bool bUseCooldownOnFailed = true;

	/**
	*	The time this task can be in Running before failing.
	*	Set <= 0.0f to never time out.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task")
	FSP_FloatParam TimeOut;

	/**
	*	Maks of notify reaction.
	*	The task will either success or failed when a notify in mask will be throw, depending on bSuccessOnNotify.
	*/
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "ESP_AIPlannerNotify"), Category = "SPlanner|Task|Notify")
	uint8 NotifyMask = 0u;

	/** How the task should react to notifications. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Notify")
	ESP_TaskNotification NotifyAction = ESP_TaskNotification::TNA_TimeOut;

	/** Callback method bound to planner. */
	UFUNCTION(Category = "SPlanner|Action|Task")
	virtual void OnNotify(USP_AIPlannerComponent* Planner, ESP_AIPlannerNotify Notify, USP_TaskInfosBase* TaskInfos);

	/** Init the Notify callback and base execution state. */
	void InitNotify(USP_AIPlannerComponent& Planner, USP_TaskInfos* TaskInfos);

	/**
	*	Blueprint event called by Begin().
	*	Require SP_TASK_BLUEPRINT_IMPLEMENTABLE (see SP_Config.h)
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName= "OnTaskBegin", Category = "SPlanner|Action|Task")
	bool K2_OnTaskBegin(USP_AIPlannerComponent* Planner);

	/**
	*	Blueprint event called by Tick().
	*	Require SP_TASK_BLUEPRINT_IMPLEMENTABLE (see SP_Config.h)
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName= "OnTaskTick", Category = "SPlanner|Action|Task")
	ESP_PlanExecutionState K2_OnTaskTick(float DeltaSeconds, USP_AIPlannerComponent* Planner);

	/**
	*	Blueprint event called by End().
	*	Require SP_TASK_BLUEPRINT_IMPLEMENTABLE (see SP_Config.h)
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName= "OnTaskEnd", Category = "SPlanner|Action|Task")
	bool K2_OnTaskEnd(USP_AIPlannerComponent* Planner);

	/**
	*	Blueprint event called by End().
	*	Require SP_TASK_BLUEPRINT_IMPLEMENTABLE (see SP_Config.h)
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName= "OnTaskCancel", Category = "SPlanner|Action|Task")
	bool K2_OnTaskCancel(USP_AIPlannerComponent* Planner);

public:
	USP_Task(const FObjectInitializer& ObjectInitializer);

	/** Gette of bUseCooldownOnFailed. */
	bool GetUseCooldownOnFailed() const;

	/** Getter of Cooldown. */
	float GetCooldown(float LODLevel = -1.0f) const;

	/** Instantiate the TaskInfos for this task. */
	virtual USP_TaskInfosBase* InstantiateInfos();

	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;

	/**
	*	The begin implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Begin(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos);

	/**
	*	The tick implementation of the task.
	*	This is executed by the PlannerComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos);

	/**
	*	The end implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool End(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos);

	/**
	*	The cancel implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Cancel(USP_AIPlannerComponent& Planner, USP_TaskInfosBase* TaskInfos);
};


/** Task info implementation for USP_Task. */
UCLASS(ClassGroup = "SPlanner|Action|Task")
class USP_TaskInfos : public USP_TaskInfosBase
{
	GENERATED_BODY()

	// Only accessible by USP_Task.
	friend USP_Task;

	ESP_PlanExecutionState BaseExecutionState = ESP_PlanExecutionState::PES_Succeed;

	float TimeOutTime = -1.0f;
	float CurrTimeOut = 0.0f;
};