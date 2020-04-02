// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableAsset/SP_IntAsset.h>

int USP_IntAsset::Get() const
{
	return Handle;
}

void USP_IntAsset::Set(int Value)
{
	// Same value.
	if (Value == Handle)
		return;

	Handle = Value;

	OnValueChange.Broadcast(Handle);
}

#if WITH_EDITOR
void USP_IntAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.GetPropertyName() == "Handle")
		OnValueChange.Broadcast(Handle);
}
#endif