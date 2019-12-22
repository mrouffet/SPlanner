#include <Miscs/FlagHelper.h>

bool USP_FlagHelper::IsFlagSet(int BitMask, uint8 Flag)
{
	return IS_FLAG_SET(BitMask, Flag);
}
void USP_FlagHelper::AddFlag(int& BitMask, uint8 Flag)
{
	ADD_FLAG(BitMask, Flag);
}
void USP_FlagHelper::RemoveFlag(int& BitMask, uint8 Flag)
{
	REMOVE_FLAG(BitMask, Flag);
}