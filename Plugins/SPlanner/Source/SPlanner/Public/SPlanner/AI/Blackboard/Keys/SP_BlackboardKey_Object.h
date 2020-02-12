#pragma once

#include <SPlanner/AI/Blackboard/Keys/SP_BlackboardKey.h>
#include "SP_BlackboardKey_Object.generated.h"

/**
*	Blackboard key object implementation.
*/
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DisplayName = "SP_BB_Object", ClassGroup = "SPlanner|Blackboard|Key")
class USP_BlackboardKey_Object : public USP_BlackboardKey
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
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|Key")
	void SetValue(UObject* Value);
};