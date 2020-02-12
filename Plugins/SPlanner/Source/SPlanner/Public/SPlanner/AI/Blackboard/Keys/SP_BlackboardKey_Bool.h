// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey.h>
#include "SP_BlackboardKey_Bool.generated.h"

/**
*	Blackboard key bool implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Bool", ClassGroup = "SPlanner|Blackboard|Key")
class USP_BlackboardKey_Bool : public USP_BlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Handle = false;

public:
	/** Getter of Handle. */
	bool GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|Key")
	void SetValue(bool Value);
};