// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_BoolParams.h>

#include <SPlanner/Debug/SP_Debug.h>

bool USP_BoolParams::Query_Implementation(const UObject* Outer) const
{
	bool Value = Super::Query_Implementation(Outer);

	for (int i = 0; i < InputPairs.Num(); ++i)
	{
		SP_CCHECK_NULLPTR(InputPairs[i].Input)

		switch (InputPairs[i].Method)
		{
		case ESP_BoolInputMethod::BIM_AND:
			Value &= InputPairs[i].Input->QueryBool(Outer);
			break;
		case ESP_BoolInputMethod::BIM_OR:
			Value |= InputPairs[i].Input->QueryBool(Outer);
			break;
		case ESP_BoolInputMethod::BIM_XOR:
			Value ^= InputPairs[i].Input->QueryBool(Outer);
			break;
		default:
			SP_LOG(Warning, "ESP_BoolInputMethod [%d] not supported yet!", static_cast<int>(InputPairs[i].Method))
			break;
		}
	}

	return Value;
}