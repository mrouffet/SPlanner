// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Params/SP_FloatParam.h>

#include <SPlanner/Base/Planner/SP_PlanState.h>

#include <SPlanner/AI/Tasks/SP_TaskMacro.h>

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
	/** The cooldown of this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task")
	FSP_FloatParam Cooldown;

	/** Should use cooldown even if the task has failed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task")
	bool bUseCooldownOnFailed = true;

	/**
	*	Construct the UserData.
	*	Children UserData struct must inherit from parent UserData struct.
	*/
	virtual void ConstructUserData(uint8* UserData);

	/**
	*	Destruct the UserData.
	*	Children UserData struct must inherit from parent UserData struct.
	*/
	virtual void DestructUserData(uint8* UserData);

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

	/** Getter of user data size to reserve. */
	virtual uint32 GetUserDataSize() const;


	bool PreCondition(const USP_PlannerComponent& Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;

	/**
	*	The begin implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Begin(USP_AIPlannerComponent& Planner, uint8* UserData);

	/**
	*	The tick implementation of the task.
	*	This is executed by the PlannerComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent& Planner, uint8* UserData);

	/**
	*	The end implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool End(USP_AIPlannerComponent& Planner, uint8* UserData);

	/**
	*	The cancel implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Cancel(USP_AIPlannerComponent& Planner, uint8* UserData);
};