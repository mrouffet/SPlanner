// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Decorator/SP_AIPlannerDecorator.h>
#include "SP_TimeOutAIDecorator.generated.h"

class USP_AIFloatParam;

/**
 *	SPlanner decorator check time out.
 *	Validate fail after time limit.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Decorator|AI")
class SPLANNER_API USP_TimeOutAIDecorator : public USP_AIPlannerDecorator
{
	GENERATED_BODY()
	
protected:
	/** Current time to check time out. */
	float CurrentTime = -1.0f;

	/** The time before failing. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	USP_AIFloatParam* TimeOutParam;

	/** Internal implementation of Validate. */
	bool Validate_Internal_Implementation(const UObject* Object) override;

	bool Tick_Validate_Internal_Implementation(float DeltaSeconds, const USP_AIPlannerComponent* Planner, const USP_TaskInfos* TaskInfos) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	USP_TimeOutAIDecorator(const FObjectInitializer& ObjectInitializer);
};