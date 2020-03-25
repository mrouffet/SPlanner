// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Curves/CurveFloat.h>

#include <SPlanner/Misc/VariableParam/SP_FloatCurveParam.h>
#include "SP_AILODFloatCurveParam.generated.h"

/**
*	float parameter struct implementation for AI LOD specialization.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Float")
class SPLANNER_API USP_AILODFloatCurveParam : public USP_FloatCurveParam
{
	GENERATED_BODY()

protected:
	float Query_Implementation(const UObject* Outer) const override;
};