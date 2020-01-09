#pragma once

#include <Async/AsyncWork.h>

class USP_PlannerComponent;

class FSP_PlanConstructTask : public FNonAbandonableTask
{
	/** The planner to construct plan. */
	USP_PlannerComponent* Planner = nullptr;

public:
	FSP_PlanConstructTask(USP_PlannerComponent* _Planner);

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSP_PlanConstructTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};