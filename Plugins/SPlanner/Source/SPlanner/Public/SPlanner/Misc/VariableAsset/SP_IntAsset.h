// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Misc/VariableAsset/SP_ScalarAsset.h>
#include "SP_IntAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_IntAssetEvent, int, Value);

/**
*	Serialized int asset.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|VariableAsset")
class SPLANNER_API USP_IntAsset : public USP_ScalarAsset
{
	GENERATED_BODY()

protected:
	/** The handled value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	int Handle = 0;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Callback thrown when Handle value change. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_IntAssetEvent OnValueChange;

	/** Getter of Handle. */
	int Get() const;

	/**
	*	Setter of Handle.
	*	Trigger OnValueChange.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Asset")
	void Set(int Value);


	bool GetBool() const override final;
	void SetBool(bool InValue) override final;

	int GetInt() const override final;
	void SetInt(int InValue) override final;

	float GetFloat() const override final;
	void SetFloat(float InValue) override final;
};