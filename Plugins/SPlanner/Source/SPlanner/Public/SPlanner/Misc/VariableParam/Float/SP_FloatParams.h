// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableParam/Float/SP_FloatParam.h>
#include "SP_FloatParams.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Float")
class SPLANNER_API USP_FloatParams : public USP_FloatParam
{
	GENERATED_BODY()

protected:
	float Query_Implementation(const UObject* Outer) const override;

public:
	/** Whether to compute the average value of all inputs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseAverage = false;

	/**
	*	Additionnal input values.
	*	The average input value will be computed and then multiplied to the current value.
	*/
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<USP_ScalarParam*> Inputs;
};