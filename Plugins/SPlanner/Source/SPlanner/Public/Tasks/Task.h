#pragma once

#include <Miscs/Settings.h>
#include <Miscs/PlanState.h>
#include <Miscs/Flags/PlannerFlags.h>

#include <Tasks/TaskMacro.h>

#include <Engine/DataAsset.h>
#include "Task.generated.h"

class USP_Goal;
class USP_PlannerComponent;

/**
 *	Planner task base.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Task")
class SPLANNER_API USP_Task : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** Should use cooldown even if the task has failed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bUseCooldownOnFailed = true;

	/** The cooldown of this task. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	float Cooldown = 0.0f;

	/** Blueprint event called by Begin(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Task")
	void OnBegin(USP_PlannerComponent* Planner);

	/** Blueprint event called by Tick(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Task")
	void OnTick(float DeltaSeconds, USP_PlannerComponent* Planner);

	/** Blueprint event called by End(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Task")
	void OnEnd(USP_PlannerComponent* Planner);

	/** Blueprint event called by End(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Task")
	void OnCancel(USP_PlannerComponent* Planner);

public:
	/** Gette of bUseCooldownOnFailed. */
	bool GetUseCooldownOnFailed() const;

	/** Getter of Cooldown. */
	float GetCooldown() const;

	/** Getter of user data size to reserve. */
	virtual uint32 GetUserDataSize() const;

	/**
	*	The pre condition of the task.
	*	Must be return true to execute the task.
	*	This is called by the Planner to generate an action plan.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task")
	virtual bool PreCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const;

	/**
	*	The post condition of the task.
	*	Return new planner flags to check for next pre-conditions.
	*	This is executed by the Planner to generate an action plan.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Task")
	virtual FSP_PlannerFlags PostCondition(const USP_PlannerComponent* Planner, FSP_PlannerFlags PlannerFlags) const;

	/**
	*	The begin implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Begin(USP_PlannerComponent* Planner, uint8* UserData);

	/**
	*	The tick implementation of the task.
	*	This is executed by the PlannerComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Tick(float DeltaSeconds, USP_PlannerComponent* Planner, uint8* UserData);

	/**
	*	The end implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual ESP_PlanExecutionState End(USP_PlannerComponent* Planner, uint8* UserData);

	/**
	*	The cancel implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Cancel(USP_PlannerComponent* Planner, uint8* UserData);
};