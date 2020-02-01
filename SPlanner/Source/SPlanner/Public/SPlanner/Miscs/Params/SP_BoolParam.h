// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Miscs/Assets/SP_BoolAsset.h>

#include <SPlanner/Miscs/Params/SP_Param.h>
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

USTRUCT(BlueprintType, Category = "SPlanner|Misc|Param")
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
USTRUCT(BlueprintType, Category = "SPlanner|Misc|Param")
struct SPLANNER_API FSP_BoolParam : public FSP_Param
{
	GENERATED_BODY()

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool Default = false;

	/** Additionnal input values. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FSP_BoolInputPair> InputPairs;

	FSP_BoolParam() = default;

	/**
	*	Getter of value using Curve with valid LODLevel, otherwise Default.
	*	Multiply with all InputWeights.
	*/
	bool Get() const;
};