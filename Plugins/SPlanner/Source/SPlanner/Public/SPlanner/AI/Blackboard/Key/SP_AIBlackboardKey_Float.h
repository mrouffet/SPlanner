// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/Key/SP_AIBlackboardKey.h>
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly) // No category on EditInlineNew object.
	float Handle = 0.0f;

public:
	/** Getter of Handle. */
	float GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI|Key")
	void SetValue(float Value);

	bool Compare(const USP_AIBlackboardKey* OtherKey) override final;
	void ResetValue(const USP_AIBlackboardKey* OriginalKey) override final;
	USP_AIBlackboardKey* CreateInstance(USP_AIBlackboardComponent* Outer) override final;
};