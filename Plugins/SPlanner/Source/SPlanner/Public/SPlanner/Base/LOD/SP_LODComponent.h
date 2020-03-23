// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_ActorComponent.h>
#include "SP_LODComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSP_LODDelegate);

/**
*	SPlanner LOD base behavior.
*	Use radius and collision with players.
*/
UCLASS(Abstract, BlueprintType, Blueprintable, DisplayName = "SP_LODComponent", ClassGroup = "SPlanner|LOD")
class SPLANNER_API USP_LODComponent : public USP_ActorComponent
{
	GENERATED_BODY()

protected:
	/** Current LOD level. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	float LODLevel = -1.0f;

	/** The current list of objects to check LOD distance with. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<USceneComponent*> Objects;

	/**
	*	Fill Objects implementation.
	*	Called in BeginPlay().
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|LOD")
	void FillObjects();

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** Radius of the LOD activation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPlanner")
	float Radius = 10000.0f;

	/** Callback events trigger when first object enter zone. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|LOD")
	FSP_LODDelegate OnActive;

	/** Callback events trigger when last object exit zone. */
	UPROPERTY(BlueprintAssignable, Category = "SPlanner|LOD")
	FSP_LODDelegate OnInactive;

	USP_LODComponent(const FObjectInitializer& ObjectInitializer);

	/** Whether this LOD is active or every Objects are out of range. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD")
	bool IsLODActive() const;

	/** Getter of LODLevel */
	float GetLODLevel() const;
};