// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <SPlanner/Framework/SP_ActorComponent.h>
#include "SP_ZoneComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSP_ZoneDelegate, UPrimitiveComponent*, Object);

/**
 *	Zone base implementation: enter / exit callbacks.
 *	Use handled shape and collision with objects to trigger events (OnEnter / OnExit).
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "SP_Zone", ClassGroup = "SPlanner|Zone", meta = (BlueprintSpawnableComponent))
class SPLANNER_API USP_ZoneComponent : public USP_ActorComponent
{
	GENERATED_BODY()
	
protected:
	/** The handled primitive used for collision detection. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	UPrimitiveComponent* Handle = nullptr;

	/** Tags of objects that can react with this while overlapping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	TArray<FName> Tags;

	/** The current list of objects inside the zone. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "SPlanner")
	TArray<UPrimitiveComponent*> Objects;

	/** Callback function bound to OnComponentBeginOverlap. */
	UFUNCTION(Category = "SPlanner|Zone")
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** Callback function bound to OnComponentEndOverlap. */
	UFUNCTION(Category = "SPlanner|Zone")
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	/** Callback function called when a valid object overlap and try to add it to Objects. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Zone")
	void AddObject(UPrimitiveComponent* Object);

	/** Callback function called when a valid object end overlap and try to reomve it from Objects. */
	UFUNCTION(BlueprintNativeEvent, Category = "SPlanner|Zone")
	void RemoveObject(UPrimitiveComponent* Object);

	void InitializeComponent() override;
	void UninitializeComponent() override;

public:
	/** Callback events trigger when this actor enter the camera's range (ie BeginOverlap)*/
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_ZoneDelegate OnEnter;

	/** Callback events trigger when this actor exit the camera's range (ie EndOverlap)*/
	UPROPERTY(BlueprintAssignable, Category = "SPlanner")
	FSP_ZoneDelegate OnExit;

	USP_ZoneComponent(const FObjectInitializer& ObjectInitializer);

	/**
	*	Setter of Handle.
	*	Bind collision events.
	*/
	UFUNCTION(BlueprintCallable, Category = "SPlanner|Zone")
	void SetHandle(UPrimitiveComponent* Primitive);
};
