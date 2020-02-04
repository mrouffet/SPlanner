#pragma once

#include <SPlanner/AI/Tasks/SP_Task.h>
#include "SP_TaskChain.generated.h"

/**
 *	Chain of Tasks.
 *	Only the cooldown of TaskChain is used by planner.
 *	Cooldown of every tasks in Tasks are not used.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_TaskChain : public USP_Task
{
	GENERATED_BODY()

	struct FSP_TaskInfos
	{
		int Index = 0;
	};

protected:
	/** The handled tasks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner|Task|Chain")
	TArray<USP_Task*> Tasks;

public:
	uint32 GetUserDataSize() const override;

	/** The pre-condition of the chain (ie: chain of pre-condition / post-condition through Steps). */
	bool PreCondition(const USP_PlannerComponent* Planner, const TArray<USP_ActionStep*>& GeneratedPlan, uint64 PlannerFlags) const override;

	/** The post-condition of the action (ie: chain of all post-condition of Steps). */
	uint64 PostCondition(const USP_PlannerComponent* Planner, uint64 PlannerFlags) const override;

	bool Begin(USP_AIPlannerComponent* Planner, uint8* UserData) override;
	ESP_PlanExecutionState Tick(float DeltaSeconds, USP_AIPlannerComponent* Planner, uint8* UserData) override;
	bool End(USP_AIPlannerComponent* Planner, uint8* UserData) override;
	
	bool Cancel(USP_AIPlannerComponent* Planner, uint8* UserData) override;
};