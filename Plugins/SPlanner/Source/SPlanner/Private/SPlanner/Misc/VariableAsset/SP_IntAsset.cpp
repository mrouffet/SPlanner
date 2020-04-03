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

bool USP_IntAsset::GetBool() const
{
	return Get();
}
void USP_IntAsset::SetBool(bool InValue)
{
	Set(static_cast<int>(InValue));
}

int USP_IntAsset::GetInt() const
{
	return Get();
}
void USP_IntAsset::SetInt(int InValue)
{
	Set(InValue);
}

float USP_IntAsset::GetFloat() const
{
	return Get();
}
void USP_IntAsset::SetFloat(float InValue)
{
	Set(static_cast<int>(InValue));
}

#if WITH_EDITOR
void USP_IntAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.GetPropertyName() == "Handle")
		OnValueChange.Broadcast(Handle);
}
#endif