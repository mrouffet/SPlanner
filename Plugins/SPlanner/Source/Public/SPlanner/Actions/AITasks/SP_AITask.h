#pragma once

#include <SPlanner/Miscs/SP_PlanState.h>

#include <SPlanner/Actions/AITasks/SP_AITaskMacro.h>

#include <SPlanner/Actions/SP_ActionStep.h>
#include "SP_AITask.generated.h"

class USP_AIPlannerComponent;

/**
 *	AI Planner task base.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "SPlanner|Action|AITask")
class SPLANNER_API USP_AITask : public USP_ActionStep
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
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|AITask")
	void OnBegin(USP_AIPlannerComponent* Planner);

	/** Blueprint event called by Tick(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|AITask")
	void OnTick(float DeltaSeconds, USP_AIPlannerComponent* Planner);

	/** Blueprint event called by End(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|AITask")
	void OnEnd(USP_AIPlannerComponent* Planner);

	/** Blueprint event called by End(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SPlanner|Action|AITask")
	void OnCancel(USP_AIPlannerComponent* Planner);

public:
	/** Gette of bUseCooldownOnFailed. */
	bool GetUseCooldownOnFailed() const;

	/** Getter of Cooldown. */
	float GetCooldown() const;

	/** Getter of user data size to reserve. */
	virtual uint32 GetUserDataSize() const;

	bool PreCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

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