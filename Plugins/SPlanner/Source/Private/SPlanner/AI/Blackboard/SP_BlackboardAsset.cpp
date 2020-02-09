#include <SPlanner/AI/BlackBoard/SP_BlackboardAsset.h>

const TArray<FSP_BlackboardEntry>& USP_BlackboardAsset::GetEntries() const
{
	return Entries;
}