// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableParam/SP_ScalarParam.h>
#include "SP_FloatParam.generated.h"

/**
*	float parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Float")
class SPLANNER_API USP_FloatParam : public USP_ScalarParam
{
	GENERATED_BODY()

protected:
	bool QueryBool_Implementation(const UObject* Outer) const override final;
	int QueryInt_Implementation(const UObject* Outer) const override final;
	float QueryFloat_Implementation(const UObject* Outer) const override final;

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float DefaultValue = 0.0f;

	/**
	*	Query the handled value.
	*	Compute DefaultValue * average inputs.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	float Query(const UObject* Outer = nullptr) const;
};