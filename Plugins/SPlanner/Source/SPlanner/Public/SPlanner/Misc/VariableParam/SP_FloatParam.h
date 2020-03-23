// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableAsset/SP_FloatAsset.h>

#include <SPlanner/Misc/VariableParam/SP_VariableParam.h>
#include "SP_FloatParam.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param")
class SPLANNER_API USP_FloatParam : public USP_VariableParam
{
	GENERATED_BODY()

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float DefaultValue = 1.0f;

	/**
	*	Additionnal input values.
	*	The average input value will be computed and then multiplied to the current value.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<USP_FloatAsset*> Inputs;

	/**
	*	Query the handled value.
	*	Compute DefaultValue * average inputs.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	float Query(const UObject* Outer = nullptr) const;
};