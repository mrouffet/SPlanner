// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/SP_SplineComponent.h>

#include <SPlanner/Debug/SP_Debug.h>

FVector USP_SplineComponent::GenerateNoiseAtPoint(int32 PointIndex) const
{
	SP_RCHECK((PointIndex >= 0 && PointIndex < Noises.Num()), FVector::ZeroVector, "PointIndex [%d] out of range [0, %d[", PointIndex, Noises.Num())

	return FVector(
		FMath::FRand() * Noises[PointIndex].X,
		FMath::FRand() * Noises[PointIndex].Y, 
		FMath::FRand() * Noises[PointIndex].Z
	);
}
FVector USP_SplineComponent::GenerateNoiseAtTime(float Time) const
{
	// TODO: Implementation.
	return FVector::ZeroVector;
}

FVector USP_SplineComponent::GetLocationAtSplinePointNoise(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace) const
{
	return GetLocationAtSplinePoint(PointIndex, CoordinateSpace) + GenerateNoiseAtPoint(PointIndex);
}
FVector USP_SplineComponent::GetLocationAtTimeNoise(float Time, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseConstantVelocity) const
{
	return GetLocationAtTime(Time, CoordinateSpace, bUseConstantVelocity) + GenerateNoiseAtTime(Time);
}

FTransform USP_SplineComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	if (InSocketName == "None")
		return Super::GetSocketTransform(InSocketName, TransformSpace);

	FTransform Result;

	float SocketInput = FCString::Atof(*InSocketName.ToString());
	SP_RCHECK(SocketInput < 0.0f, Result, "Bad socked input!")

	switch (TransformSpace)
	{
		case RTS_World:
			Result.SetScale3D(GetComponentScale());
			Result.SetLocation(GetLocationAtSplineInputKey(SocketInput, ESplineCoordinateSpace::World));
			Result.SetRotation(GetQuaternionAtSplineInputKey(SocketInput, ESplineCoordinateSpace::World));
			break;
		case RTS_Actor:
			Result.SetScale3D(GetOwner()->GetActorScale());
			Result.SetLocation(GetLocationAtSplineInputKey(SocketInput, ESplineCoordinateSpace::Local));
			Result.SetRotation(GetQuaternionAtSplineInputKey(SocketInput, ESplineCoordinateSpace::Local));
			break;
		case RTS_Component:
			Result.SetScale3D(RelativeScale3D);
			Result.SetLocation(GetLocationAtSplineInputKey(SocketInput, ESplineCoordinateSpace::Local));
			Result.SetRotation(GetQuaternionAtSplineInputKey(SocketInput, ESplineCoordinateSpace::Local));
			break;
		case RTS_ParentBoneSpace:
		default:
			SP_LOG(Error, "ERelativeTransformSpace not supported yet!")
			break;
	}

	return Result;
}

#if WITH_EDITOR
void USP_SplineComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	while (Noises.Num() < SplineCurves.Position.Points.Num())
		Noises.Add(FVector::ZeroVector);

	while (Noises.Num() > SplineCurves.Position.Points.Num())
		Noises.RemoveAt(Noises.Num() - 1);
}
#endif