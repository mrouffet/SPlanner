// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_AIBlackboardKey.h>
#include "SP_AIBlackboardKey_Object.generated.h"

/**
*	Blackboard key object implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Object", ClassGroup = "SPlanner|Blackboard|AI|Key")
class USP_AIBlackboardKey_Object : public USP_AIBlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	UObject* Handle = nullptr;

public:
	/** Getter of Handle. */
	UObject* GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI|Key")
	void SetValue(UObject* Value);

	void ResetValue(const USP_AIBlackboardKey* OriginalKey) override final;
	USP_AIBlackboardKey* CreateInstance() override final;
};