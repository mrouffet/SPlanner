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

bool USP_BoolAsset::GetBool() const
{
	return Get();
}
void USP_BoolAsset::SetBool(bool InValue)
{
	Set(InValue);
}

int USP_BoolAsset::GetInt() const
{
	return Get();
}
void USP_BoolAsset::SetInt(int InValue)
{
	Set(static_cast<bool>(InValue));
}

float USP_BoolAsset::GetFloat() const
{
	return Get();
}
void USP_BoolAsset::SetFloat(float InValue)
{
	Set(static_cast<bool>(InValue));
}

#if WITH_EDITOR
void USP_BoolAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "Handle")
		OnValueChange.Broadcast(Handle);
}
#endif