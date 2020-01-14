#pragma once

#include <Components/SphereComponent.h>
#include "SP_LODComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSP_LODDelegate);


/**
*	SPlanner LOD base behavior.
*	Use radius and collision with camera to trigger OnEnterRange / OnExitRange events.
*/
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "SPlanner|LOD")
class SPLANNER_API USP_LODComponent : public USphereComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	bool bIsInRange = false;

	/** Callback function bound to OnEnterRange. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|LOD")
	void OnEnterRange_Internal();

	/** Callback function bound to OnExitRange. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|LOD")
	void OnExitRange_Internal();

	/** Callback function bound to OnComponentBeginOverlap. */
	UFUNCTION(Category = "SPlanner|LOD")
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** Callback function bound to OnComponentEndOverlap. */
	UFUNCTION(Category = "SPlanner|LOD")
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void InitializeComponent() override;
	void UninitializeComponent() override;

public:
	/** Callback events trigger when this actor enter the camera's range (ie BeginOverlap)*/
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_LODDelegate OnEnterRange;

	/** Callback events trigger when this actor exit the camera's range (ie EndOverlap)*/
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_LODDelegate OnExitRange;

	USP_LODComponent(const FObjectInitializer& ObjectInitializer);

	/** Getter of bIsInRange. */
	bool GetIsInRange() const;

	/** Get the distance between Actor and Camera. */
	UFUNCTION(BlueprintPure, Category = "SPlanner|LOD")
	static float GetDistanceFromCamera(AActor* Actor);

	/** Get the distance between Actor and Camera. */
	static float GetDistanceFromCamera(UActorComponent* Component);
};