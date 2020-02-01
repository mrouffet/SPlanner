// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Miscs/Params/SP_BoolParam.h>

#include <SPlanner/Debug/SP_Debug.h>

bool FSP_BoolParam::Get() const
{
	bool Value = Default;

	for (int i = 0; i < InputPairs.Num(); ++i)
	{
		SP_SCCHECK_NULLPTR(InputPairs[i].Input)

		switch (InputPairs[i].Method)
		{
		case ESP_BoolInputMethod::BIM_AND:
			Value &= InputPairs[i].Input->Get();
			break;
		case ESP_BoolInputMethod::BIM_OR:
			Value |= InputPairs[i].Input->Get();
			break;
		case ESP_BoolInputMethod::BIM_XOR:
			Value ^= InputPairs[i].Input->Get();
			break;
		default:
			SP_SLOG(Warning, "ESP_BoolInputMethod [%d] not supported yet!", static_cast<int>(InputPairs[i].Method))
			break;
		}
	}

	return Value;
}