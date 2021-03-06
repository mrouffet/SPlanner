// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey.h>
#include "SP_AIBlackboardKey_Bool.generated.h"

/**
*	Blackboard key bool implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Bool", ClassGroup = "SPlanner|Blackboard|AI|Key")
class USP_AIBlackboardKey_Bool : public USP_AIBlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	bool Handle = false;

public:
	/** Getter of Handle. */
	bool GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI|Key")
	void SetValue(bool Value);

	bool Compare(const USP_AIBlackboardKey* OtherKey) override final;
	void ResetValue(const USP_AIBlackboardKey* OriginalKey) override final;
	USP_AIBlackboardKey* CreateInstance(USP_AIBlackboardComponent* Outer) override final;
};