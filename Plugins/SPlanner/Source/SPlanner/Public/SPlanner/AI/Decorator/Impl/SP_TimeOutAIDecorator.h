// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Decorator/SP_AIPlannerDecorator.h>
#include "SP_TimeOutAIDecorator.generated.h"

class USP_FloatParam;

/**
 *	SPlanner decorator check time out.
 *	Validate fail after time limit.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator|AI")
class SPLANNER_API USP_TimeOutAIDecorator : public USP_AIPlannerDecorator
{
	GENERATED_BODY()
	
protected:
	/** Times of all planners of this decorator to check time out. */
	TMap<const UObject*, float> Times;

	/** The time before failing. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_FloatParam* TimeOutParam;

	/** Internal implementation of Validate. */
	bool Validate_Internal_Implementation(const UObject* Object) override;
	void PostExecution_Internal_Implementation(const UObject* Object, bool bExecutionSuccess) override;

	bool Tick_Validate_Internal_Implementation(float DeltaSeconds, const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos) override;

public:
	USP_TimeOutAIDecorator(const FObjectInitializer& ObjectInitializer);
};