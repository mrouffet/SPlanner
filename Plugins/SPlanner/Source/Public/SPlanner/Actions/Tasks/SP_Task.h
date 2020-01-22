#pragma once

#include <SPlanner/Miscs/SP_PlanState.h>

#include <SPlanner/Actions/Tasks/SP_TaskMacro.h>

#include <SPlanner/Actions/SP_ActionStep.h>
#include "SP_Task.generated.h"

class USP_AIPlannerComponent;

/**
 *	Planner task base.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_Task : public USP_ActionStep
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
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|Task")
	void OnBegin(USP_AIPlannerComponent* Planner);

	/** Blueprint event called by Tick(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|Task")
	void OnTick(float DeltaSeconds, USP_AIPlannerComponent* Planner);

	/** Blueprint event called by End(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|Task")
	void OnEnd(USP_AIPlannerComponent* Planner);

	/** Blueprint event called by End(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|Task")
	void OnCancel(USP_AIPlannerComponent* Planner);

public:
	/** Gette of bUseCooldownOnFailed. */
	bool GetUseCooldownOnFailed() const;

	/** Getter of Cooldown. */
	float GetCooldown() const;

	/** Getter of user data size to reserve. */
	virtual uint32 GetUserDataSize() const;

	/**
	*	The begin implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Begin(USP_AIPlannerComponent* Planner, uint8* UserData);

	/**
	*	The tick implementation of the task.
	*	This is executed by the PlannerComponent (main thread).
	*/
	virtual ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData);

	/**
	*	The end implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool End(USP_AIPlannerComponent* Planner, uint8* UserData);

	/**
	*	The cancel implementation of the task.
	*	This is executed by the SP_PlannerComponent (main thread).
	*/
	virtual bool Cancel(USP_AIPlannerComponent* Planner, uint8* UserData);
};