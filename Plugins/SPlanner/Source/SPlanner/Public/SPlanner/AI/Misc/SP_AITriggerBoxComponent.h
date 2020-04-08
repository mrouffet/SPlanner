// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Components/BoxComponent.h>
#include "SP_AITriggerBoxComponent.generated.h"

/**
*	Trigger box for AI Planner Notify(Trigger).
*/
UCLASS(BlueprintType, Blueprintable, DisplayName = "SP_AITriggerBox", meta = (BlueprintSpawnableComponent), ClassGroup = "SPlanner|AI|Misc")
class SPLANNER_API USP_AITriggerBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

protected:
	void InitializeComponent() override;
	void UninitializeComponent() override;

	/** Whether notify should be triggered on enter (Begin overlap). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bNotifyOnEnter = true;

	/** Whether notify should be triggered on exit (End overlap). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SPlanner")
	bool bNotifyOnExit = false;

	void NotifyActor(AActor* OtherActor);

public:
	USP_AITriggerBoxComponent(const FObjectInitializer& ObjectInitializer);

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
};