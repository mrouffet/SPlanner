// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Blackboard/SP_BlackboardComponent.h>
#include "SP_AIBlackboardComponent.generated.h"

class USP_AIBlackboardKey;
class USP_AIBlackboardObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBBoolDelegate, const FName&, Name, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBIntDelegate, const FName&, Name, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBFloatDelegate, const FName&, Name, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBVectorDelegate, const FName&, Name, FVector, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBRotatorDelegate, const FName&, Name, FRotator, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBFNameDelegate, const FName&, Name, FName, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSP_AIBBObjectDelegate, const FName&, Name, UObject*, Value);


/**
 *	SPlanner AI Blackboard component implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard|AI")
class SPLANNER_API USP_AIBlackboardComponent : public USP_BlackboardComponent
{
	GENERATED_BODY()

protected:
	/**
	*	Blackboard instantiated keys by entry name.
	*	Must be UPROPERTY() to avoid USP_AIBlackboardKey* garbage collection.
	*/
	UPROPERTY()
	TMap<FName, USP_AIBlackboardKey*> Keys;

	void InitializeBlackboard_Implementation() override;
	void UnInitializeBlackboard_Implementation() override;

public:
	/** Callback thrown on bool value change (SetBool()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBBoolDelegate OnBoolValueChange;

	/** Callback thrown on int value change (SetInt()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBIntDelegate OnIntValueChange;

	/** Callback thrown on float value change (SetFloat()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBFloatDelegate OnFloatValueChange;

	/** Callback thrown on Vector value change (SetVector()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBVectorDelegate OnVectorValueChange;

	/** Callback thrown on Rotator value change (SetRotator()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBRotatorDelegate OnRotatorValueChange;

	/** Callback thrown on FName value change (SetFName()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBFNameDelegate OnFNameValueChange;

	/** Callback thrown on Object value change (SetObject()). */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|Blackboar|AI")
	FSP_AIBBObjectDelegate OnObjectValueChange;


	/** Getter of bool value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	bool GetBool(const FName& EntryName) const;

	/** Setter of bool value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetBool(const FName& EntryName, bool Value);


	/** Getter of int value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	int GetInt(const FName& EntryName) const;

	/** Setter of int value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetInt(const FName& EntryName, int Value);


	/** Getter of float value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	float GetFloat(const FName& EntryName) const;

	/** Setter of float value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetFloat(const FName& EntryName, float Value);


	/** Getter of FVector value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	const FVector& GetVector(const FName& EntryName) const;

	/** Setter of FVector value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetVector(const FName& EntryName, const FVector& Value);


	/** Getter of FRotator value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	const FRotator& GetRotator(const FName& EntryName) const;

	/** Setter of FRotator value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetRotator(const FName& EntryName, const FRotator& Value);


	/** Getter of FName value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	const FName& GetFName(const FName& EntryName) const;

	/** Setter of FName value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetFName(const FName& EntryName, const FName& Value);

	/** Getter of Object value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	USP_AIBlackboardObject* GetObject(const FName& EntryName) const;

	/** Getter of casted Object value registered as EntryName. */
	template <typename T>
	T* GetObject(const FName& EntryName) const
	{
		return Cast<T>(GetObject(EntryName));
	}

	/** Setter of Object value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetObject(const FName& EntryName, USP_AIBlackboardObject* Value);

	/** Reset the value to default (Blackboard Asset). */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void ResetValue(const FName& EntryName);

	/** Reset this blackboard. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Blackboard")
	void Reset();

	/**
	*	Reset this blackboard due to a plan cancel.
	*	Reset only keys with bShouldResetOnPlanCancel = true.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Blackboard")
	void ResetPlanCancel();
};