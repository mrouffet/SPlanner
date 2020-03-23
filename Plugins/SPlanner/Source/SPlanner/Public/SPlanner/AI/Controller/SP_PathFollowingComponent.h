// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once

#include <Navigation/PathFollowingComponent.h>
#include "SP_PathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "Planner", ClassGroup = "SPlanner|Planner")
class SPLANNER_API USP_PathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
	
protected:
	void InitializeComponent() override;

	void AbortMove(const UObject& Instigator,
		FPathFollowingResultFlags::Type AbortFlags,
		FAIRequestID RequestID,
		EPathFollowingVelocityMode VelocityMode) override;

	FAIRequestID RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath) override;

public:
	/** Should enable steering. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPlanner")
	bool bUseSteering = false;

	USP_PathFollowingComponent(const FObjectInitializer& ObjectInitializer);
};