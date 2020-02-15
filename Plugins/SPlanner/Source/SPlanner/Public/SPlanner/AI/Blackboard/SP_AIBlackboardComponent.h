// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Base/Blackboard/SP_BlackboardComponent.h>
#include "SP_AIBlackboardComponent.generated.h"

class USP_AIBlackboardKey;

/**
 *	SPlanner AI Blackboard component implementation
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Blackboard|AI")
class SPLANNER_API USP_AIBlackboardComponent : public USP_BlackboardComponent
{
	GENERATED_BODY()

protected:
	/** Blackboard instantiated keys by entry name. */
	TMap<FName, USP_AIBlackboardKey*> Keys;

	void InitializeBlackboard_Implementation() override;
	void UnInitializeBlackboard_Implementation() override;

public:
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
	UObject* GetObject(const FName& EntryName) const;

	/** Getter of casted Object value registered as EntryName. */
	template <typename T>
	T* GetObject(const FName& EntryName) const
	{
		return Cast<T>(GetObject(EntryName));
	}

	/** Setter of Object value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void SetObject(const FName& EntryName, UObject* Value);

	/** Reset the value to default (Blackboard Asset). */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard|AI")
	void ResetValue(const FName& EntryName);
};