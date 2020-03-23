// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableAsset/SP_BoolAsset.h>

bool USP_BoolAsset::Get() const
{
	return Handle;
}

void USP_BoolAsset::Set(bool Value)
{
	// Same value.
	if (Value == Handle)
		return;

	Handle = Value;

	OnValueChange.Broadcast(Handle);
}

#if WITH_EDITOR
void USP_BoolAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OnValueChange.Broadcast(Handle);
}
#endif