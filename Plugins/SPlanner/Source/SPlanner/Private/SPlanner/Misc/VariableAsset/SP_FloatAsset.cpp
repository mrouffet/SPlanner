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

bool USP_FloatAsset::GetBool() const
{
	return Get() != 0.0f;
}
void USP_FloatAsset::SetBool(bool InValue)
{
	Set(static_cast<float>(InValue));
}

int USP_FloatAsset::GetInt() const
{
	return static_cast<int>(Get());
}
void USP_FloatAsset::SetInt(int InValue)
{
	Set(static_cast<float>(InValue));
}

float USP_FloatAsset::GetFloat() const
{
	return Get();
}
void USP_FloatAsset::SetFloat(float InValue)
{
	Set(InValue);
}

#if WITH_EDITOR
void USP_FloatAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "Handle")
		OnValueChange.Broadcast(Handle);
}
#endif