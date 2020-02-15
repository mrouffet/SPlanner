// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/AI/Target/SP_TargetState.h>

#include <SPlanner/AI/Blackboard/SP_AIBlackboardObject.h>
#include "SP_Target.generated.h"

class USP_POIComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_TargetEvent, USP_Target*, Target);

/**
*	Implementation of planner target.
*/
UCLASS(BlueprintType, Blueprintable, ClassGroup = "SPlanner|Target", DisplayName = "SP_Target")
class SPLANNER_API USP_Target : public USP_AIBlackboardObject
{
	GENERATED_BODY()

protected:
	/** Target depending on State. */
	union
	{
		FVector Position;
		AActor* Actor;
		APawn* Player;
		USP_POIComponent* POI;
	};

	/** The current target state. */
	UPROPERTY(BlueprintReadOnly, Category = "SPlanner")
	ESP_TargetState State = ESP_TargetState::TS_None;

public:
	/** Callback thrown when target is changed. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_TargetEvent OnTargetChange;

	USP_Target(const FObjectInitializer& ObjectInitializer);
	
	// DO NOT USE. Unreal internal use only.
	USP_Target(FVTableHelper& Helper);


	/** Getter of State. */
	ESP_TargetState GetState() const;

	/** Whether target is a position. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsPosition() const;

	/** Whether target is an actor (or a player). */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsActor() const;

	/** Whether target is a player. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsPlayer() const;

	/** Whether target is a position. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsPOI() const;


	/** Getter of Position */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	const FVector& GetPosition() const;

	/** Getter of position for any kind of target. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	FVector GetAnyPosition() const;

	/** Getter of Actor */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	AActor* GetActor() const;

	/** Getter of any kind of actor */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	AActor* GetAnyActor() const;

	/** Getter of Actor as player */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	APawn* GetPlayer() const;

	/** Getter of TargetPOI */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	USP_POIComponent* GetPOI() const;


	/** Setter of TargetPosition */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetPosition(const FVector& InPosition);

	/** Setter of Actor */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetActor(AActor* InActor);

	/** Setter of Actor as player */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetPlayer(APawn* InPlayer);

	/** Setter of POI */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void SetPOI(USP_POIComponent* InPOI);


	/** Return whether the target is valid. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|Target")
	bool IsValid() const;

	/** Reset to EAITargetState::AITS_None. */
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Target")
	void Clear();

	void Reset(USP_AIBlackboardObject* OriginalObject) override;
};