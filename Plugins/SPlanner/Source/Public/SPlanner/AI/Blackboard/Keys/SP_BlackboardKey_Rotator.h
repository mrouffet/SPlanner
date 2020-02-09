#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey.h>
#include "SP_BlackboardKey_Rotator.generated.h"

/**
*	Blackboard key rotator implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Rotator", ClassGroup = "SPlanner|Blackboard|Key")
class USP_BlackboardKey_Rotator : public USP_BlackboardKey
{
	GENERATED_BODY()

protected:
	/** Handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator Handle;

public:
	/** Getter of Handle. */
	const FRotator& GetValue() const;

	/** Setter of Handle. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|Key")
	void SetValue(const FRotator& Value);
};