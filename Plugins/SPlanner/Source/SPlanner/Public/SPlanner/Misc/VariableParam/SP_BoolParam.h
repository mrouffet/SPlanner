// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableParam/SP_VariableParam.h>
#include "SP_BoolParam.generated.h"

/**
*	bool parameter struct implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, ClassGroup = "SPlanner|Misc|Param|Bool")
class SPLANNER_API USP_BoolParam : public USP_VariableParam
{
	GENERATED_BODY()

public:
	/** The default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool DefaultValue = false;

	/**
	*	Getter of value using Curve with valid LODLevel, otherwise Default.
	*	Multiply with all InputWeights.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SPlanner")
	bool Query(const UObject* Outer = nullptr) const;
};


UENUM(BlueprintType, Category = "SPlanner|Misc|Param|Bool")
enum class ESP_BoolInputMethod : uint8
{
	/** Apply using AND operator. */
	BIM_AND					UMETA(DisplayName = "AND"),

	/** Apply using OR operator. */
	BIM_OR					UMETA(DisplayName = "OR"),

	/** Apply using XOR operator. */
	BIM_XOR					UMETA(DisplayName = "XOR"),
};