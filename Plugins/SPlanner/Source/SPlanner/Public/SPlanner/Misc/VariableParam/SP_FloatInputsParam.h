// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableAsset/SP_FloatAsset.h>

#include <SPlanner/Misc/VariableParam/SP_FloatParam.h>
#include "SP_FloatInputsParam.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Float")
class SPLANNER_API USP_FloatInputsParam : public USP_FloatParam
{
	GENERATED_BODY()

	float Query_Implementation(const UObject* Outer) const override;

public:
	/**
	*	Additionnal input values.
	*	The average input value will be computed and then multiplied to the current value.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<USP_FloatAsset*> Inputs;
};