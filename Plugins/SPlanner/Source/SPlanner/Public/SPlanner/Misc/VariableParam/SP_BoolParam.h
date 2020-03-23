// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableAsset/SP_BoolAsset.h>

#include <SPlanner/Misc/VariableParam/SP_VariableParam.h>
#include "SP_BoolParam.generated.h"

UENUM(BlueprintType, Category = "SPlanner|Misc|Param")
enum class ESP_BoolInputMethod : uint8
{
	/** Apply using AND operator. */
	BIM_AND					UMETA(DisplayName = "AND"),

	/** Apply using OR operator. */
	BIM_OR					UMETA(DisplayName = "OR"),

	/** Apply using XOR operator. */
	BIM_XOR					UMETA(DisplayName = "XOR"),
};

USTRUCT(BlueprintType, Category = "SPlanner|Misc|VariableParam")
struct SPLANNER_API FSP_BoolInputPair
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
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|VariableParam")
class SPLANNER_API USP_BoolParam : public USP_VariableParam
{
	GENERATED_BODY()

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool DefaultValue = false;

	/** Additionnal input values. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	TArray<FSP_BoolInputPair> InputPairs;

	/**
	*	Getter of value using Curve with valid LODLevel, otherwise Default.
	*	Multiply with all InputWeights.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	bool Query(const UObject* Outer = nullptr) const;
};