// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include "SP_PathFollowingComponent.h"

USP_PathFollowingComponent::USP_PathFollowingComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void USP_PathFollowingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	bStopMovementOnFinish = !bUseSteering;
}

void USP_PathFollowingComponent::AbortMove(const UObject& Instigator,
	FPathFollowingResultFlags::Type AbortFlags,
	FAIRequestID RequestID,
	EPathFollowingVelocityMode VelocityMode)
{
	// No Super call: Fully override for bUseSteering implementation.
	//Super::AbortMove(Instigator, AbortFlags, RequestID, VelocityMode);

	if ((Status != EPathFollowingStatus::Idle) && RequestID.IsEquivalent(GetCurrentRequestId()))
	{
		bStopMovementOnFinish = !bUseSteering && (VelocityMode == EPathFollowingVelocityMode::Reset);
		OnPathFinished(FPathFollowingResult(EPathFollowingResult::Aborted, AbortFlags & FPathFollowingResultFlags::UserAbortFlagMask));
	}
}

FAIRequestID USP_PathFollowingComponent::RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath)
{
	// Force reset StopMovementOnFinish.
	bStopMovementOnFinish = !bUseSteering;

	FAIRequestID Result = Super::RequestMove(RequestData, InPath);

	// Force reset StopMovementOnFinish again.
	bStopMovementOnFinish = !bUseSteering;

	return Result;
}