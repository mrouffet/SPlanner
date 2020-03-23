// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableAsset/SP_FloatAsset.h>

float USP_FloatAsset::Get() const
{
	return Handle;
}

void USP_FloatAsset::Set(float Value)
{
	// Same value.
	if (FMath::IsNearlyEqual(Handle, Value))
		return;

	Handle = Value;

	OnValueChange.Broadcast(Handle);
}

#if WITH_EDITOR
void USP_FloatAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OnValueChange.Broadcast(Handle);
}
#endif