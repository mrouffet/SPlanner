// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/VariableParam/SP_FloatCurveParam.h>

#include <SPlanner/Debug/SP_Debug.h>

USP_FloatCurveParam::USP_FloatCurveParam(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Multiplier.GetRichCurve()->AddKey(0.0f, 1.0f);
}

float USP_FloatCurveParam::Query_Implementation(const UObject* Outer) const
{
	float Value = -1.0f;

	if (const USP_FloatCurveParamInfos* const Infos = Cast<USP_FloatCurveParamInfos>(Outer))
	{
		// Forward Infos' Outer.
		Value = Super::Query_Implementation(Infos->Outer);

		if(!Multiplier.GetRichCurveConst()->IsEmpty())
			Value *= Multiplier.GetRichCurveConst()->Eval(Infos->CurveTime);
	}
	else
	{
		SP_LOG(Warning, "Outer is not of type USP_FloatCurveParamInfos")

		Value = Super::Query_Implementation(Outer);
	}

	return Value;
}