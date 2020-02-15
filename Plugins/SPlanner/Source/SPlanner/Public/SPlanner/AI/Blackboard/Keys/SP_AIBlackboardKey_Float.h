// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey.h>
#include "SP_AIBlackboardKey_Float.generated.h"

/**
*	Blackboard key float implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Float", ClassGroup = "SPlanner|Blackboard|AI|Key")
class USP_AIBlackboardKey_Float : public USP_AIBlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Handle = 0.0f;

public:
	/** Getter of Handle. */
	float GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI|Key")
	void SetValue(float Value);

	void CopyValue(const USP_AIBlackboardKey* Other) override final;
};