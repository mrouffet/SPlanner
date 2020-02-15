// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey.h>
#include "SP_AIBlackboardKey_Vector.generated.h"

/**
*	Blackboard key vector implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Vector", ClassGroup = "SPlanner|Blackboard|AI|Key")
class USP_AIBlackboardKey_Vector : public USP_AIBlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Handle;

public:
	/** Getter of Handle. */
	const FVector& GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI|Key")
	void SetValue(const FVector& Value);

	void ResetValue(const USP_AIBlackboardKey* OriginalKey) override final;
	USP_AIBlackboardKey* CreateInstance() override final;
};