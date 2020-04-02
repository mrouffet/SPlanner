// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableParam/SP_ScalarParam.h>
#include "SP_IntParam.generated.h"

/**
*	int parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Int")
class SPLANNER_API USP_IntParam : public USP_ScalarParam
{
	GENERATED_BODY()

protected:
	bool QueryBool_Implementation(const UObject* Outer) const override final;
	int QueryInt_Implementation(const UObject* Outer) const override final;
	float QueryFloat_Implementation(const UObject* Outer) const override final;

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	int DefaultValue = false;

	/** Getter of value. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	int Query(const UObject* Outer = nullptr) const;
};