// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#include <SPlanner/Misc/SP_FlagHelper.h>

bool USP_FlagHelper::IsFlagSet(int BitMask, uint8 Flag)
{
	return SP_IS_FLAG_SET(BitMask, Flag);
}

void USP_FlagHelper::SetFlag(int& BitMask, uint8 Flag)
{
	SP_SET_FLAG(BitMask, Flag);
}

void USP_FlagHelper::AddFlag(int& BitMask, uint8 Flag)
{
	SP_ADD_FLAG(BitMask, Flag);
}

void USP_FlagHelper::RemoveFlag(int& BitMask, uint8 Flag)
{
	SP_REMOVE_FLAG(BitMask, Flag);
}