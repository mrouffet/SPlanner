// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <AssetTypeActions_Base.h>
#include <Toolkits/IToolkitHost.h>


class FSP_AssetTypeActions_AIAsset : public FAssetTypeActions_Base
{
public:
	uint32 GetCategories() override;
};
