#pragma once

#include <Tasks/Task.h>
#include "TargetTask.generated.h"

class USP_TargetComponent;

/**
 *	Target task base.
 */
UCLASS(Abstract, ClassGroup = "SPlanner|Task|Target")
class SPLANNER_API USP_TargetTask : public USP_Task
{
	GENERATED_BODY()

protected:
	/**
	*	The pre condition of the task for Target spec.
	*	Must be return true to execute the task.
	*	This is called by the Planner to generate an action plan.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	bool PreCondition_Internal(const USP_PlannerComponent* Planner, const USP_TargetComponent* Target, int PlannerFlags) const;

	/**
	*	The post condition of the task for Target spec.
	*	Must be return true to execute the task.
	*	This is called by the Planner to generate an action plan.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task|Target")
	int PostCondition_Internal(const USP_PlannerComponent* Planner, const USP_TargetComponent* Target, int PlannerFlags) const;

	/**
	*	The begin implementation of the task for Target spec.
	*	This is executed by the SP_PlannableComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Begin_Internal(USP_PlannerComponent* Planner, USP_TargetComponent* Target, uint8* UserData);

	/**
	*	The implementation of the task  for Target spec.
	*	This is executed by the PlannableComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Tick_Internal(float DeltaSeconds, USP_PlannerComponent* Planner, USP_TargetComponent* Target, uint8* UserData);

	/**
	*	The begin implementation of the task  for Target spec.
	*	This is executed by the SP_PlannableComponent (main thread).
	*/
	virtual ESP_PlanExecutionState End_Internal(USP_PlannerComponent* Planner, USP_TargetComponent* Target, uint8* UserData);

public:
	bool PreCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const override;
	int PostCondition(const USP_PlannerComponent* Planner, int PlannerFlags) const override;

	ESP_PlanExecutionState Begin(USP_PlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState End(USP_PlannerComponent* Planner, uint8* UserData) override;
};