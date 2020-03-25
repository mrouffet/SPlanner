// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableAsset/SP_BoolAsset.h>

#include <SPlanner/Misc/VariableParam/SP_BoolParam.h>
#include "SP_BoolInputsParam.generated.h"

USTRUCT(BlueprintType, Category = "SPlanner|Misc|Param|Bool")
struct SPLANNER_API FSP_BoolInputAssetPair
{
	GENERATED_BODY()

	/** The input bool value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	USP_BoolAsset* Input = nullptr;

	/** The method to apply Input. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	ESP_BoolInputMethod Method = ESP_BoolInputMethod::BIM_OR;
};

/**
*	bool parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Bool")
class SPLANNER_API USP_BoolInputsParam : public USP_BoolParam
{
	GENERATED_BODY()

protected:
	bool Query_Implementation(const UObject* Outer) const override;

public:
	/** Additionnal input values. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<FSP_BoolInputAssetPair> InputPairs;
};