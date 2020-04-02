// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Task/Impl/Idle/SP_IdleBaseTask.h>
#include "SP_IdleCurveTask.generated.h"

class USP_FloatCurveParam;

/**
 *	Idle task implementation.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Action|Task")
class SPLANNER_API USP_IdleCurveTask : public USP_IdleBaseTask
{
	GENERATED_BODY()
	
protected:
	/** The X value of the minimum curve. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	USP_FloatCurveParam* MinCurve = nullptr;

	/** The X value of the maximum curve. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "SPlanner|Task|Idle")
	USP_FloatCurveParam* MaxCurve = nullptr;

	float GenerateTime_Implementation(const USP_AIPlannerComponent* Planner) override;
};