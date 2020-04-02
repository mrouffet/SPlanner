// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableParam/SP_VariableParam.h>
#include "SP_ScalarParam.generated.h"

/**
*	Parameter scalar base implementation.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param")
class SPLANNER_API USP_ScalarParam : public USP_VariableParam
{
	GENERATED_BODY()

public:
	/** Getter of value as bool. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	bool QueryBool(const UObject* Outer = nullptr) const;

	/** Getter of value as int. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	int QueryInt(const UObject* Outer = nullptr) const;

	/** Getter of value as int. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	float QueryFloat(const UObject* Outer = nullptr) const;
};