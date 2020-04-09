// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Misc/VariableParam/Float/SP_FloatCurveParam.h>
#include "SP_AIDistToTargetFloatCurveParam.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|AI|Misc")
class SPLANNER_API USP_AIDistToTargetFloatCurveParam : public USP_FloatCurveParam
{
	GENERATED_BODY()

protected:
	float Query_Implementation(const UObject* Outer) const override;

public:
	/** The target entry name to access from blackboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	FName TargetEntryName = "None";
};