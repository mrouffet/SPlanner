#pragma once

#include <SPlanner/Actions/AITasks/SP_AITask.h>
#include "SP_AITaskChain.generated.h"

/**
 *	Chain of Tasks.
 *	Only the cooldown of TaskChain is used by planner.
 *	Cooldown of every tasks in Tasks are not used.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|AITask")
class SPLANNER_API USP_AITaskChain : public USP_AITask
{
	GENERATED_BODY()

	struct FSP_TaskInfos
	{
		int Index = 0;
		TArray<uint8> UserData;
	};

protected:
	/** The handled tasks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<USP_AITask*> Tasks;

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