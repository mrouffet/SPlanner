// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableParam/Int/SP_IntParam.h>
#include "SP_IntParams.generated.h"

/**
*	bool parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Int")
class SPLANNER_API USP_IntParams : public USP_IntParam
{
	GENERATED_BODY()

protected:
	int Query_Implementation(const UObject* Outer) const override;

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