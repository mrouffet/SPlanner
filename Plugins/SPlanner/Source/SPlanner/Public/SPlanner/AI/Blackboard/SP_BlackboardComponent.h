// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Components/ActorComponent.h>
#include "SP_BlackboardComponent.generated.h"

class USP_BlackboardKey;
class USP_BlackboardAsset;

/**
*	Blackboard component, holding a duplicated SP_BlackboardAsset.
*/
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent), DisplayName = "SP_Blackboard", ClassGroup = "SPlanner|Blackboard")
class SPLANNER_API USP_BlackboardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** BlackboardKeys by entry name. */
	TMap<FName, USP_BlackboardKey*> Keys;

public:
	/** Create a duplicated asset to use it. */
	void InitBlackboard(const USP_BlackboardAsset* BlackboardAssetSource);


	/** Getter of bool value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	bool GetBool(const FName& EntryName) const;

	/** Setter of bool value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetBool(const FName& EntryName, bool Value);


	/** Getter of int value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	int GetInt(const FName& EntryName) const;

	/** Setter of int value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetInt(const FName& EntryName, int Value);


	/** Getter of float value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	float GetFloat(const FName& EntryName) const;

	/** Setter of float value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetFloat(const FName& EntryName, float Value);


	/** Getter of FVector value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	const FVector& GetVector(const FName& EntryName) const;

	/** Setter of FVector value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetVector(const FName& EntryName, const FVector& Value);


	/** Getter of FRotator value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	const FRotator& GetRotator(const FName& EntryName) const;

	/** Setter of FRotator value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetRotator(const FName& EntryName, const FRotator& Value);


	/** Getter of FName value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	const FName& GetFName(const FName& EntryName) const;

	/** Setter of FName value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetFName(const FName& EntryName, const FName& Value);

	/** Getter of Object value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	UObject* GetObject(const FName& EntryName) const;

	/** Getter of casted Object value registered as EntryName. */
	template <typename T>
	T* GetObject(const FName& EntryName) const
	{
		return Cast<T>(GetObject(EntryName));
	}

	/** Setter of Object value registered as EntryName. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Blackboard")
	void SetObject(const FName& EntryName, UObject* Value);
};