// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableAsset/SP_VariableAsset.h>
#include "SP_ScalarAsset.generated.h"

/**
*	Serialized scalar asset base implementation.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = "SPlanner|VariableAsset")
class SPLANNER_API USP_ScalarAsset : public USP_VariableAsset
{
	GENERATED_BODY()

public:
	/** Getter of value as bool. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual bool GetBool() const;

	/** Setter of value as bool. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual void SetBool(bool InValue);

	/** Getter of value as int. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual int GetInt() const;

	/** Setter of value as int. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual void SetInt(int InValue);

	/** Getter of value as int. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual float GetFloat() const;

	/** Setter of value as int. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner")
	virtual void SetFloat(float InValue);
};