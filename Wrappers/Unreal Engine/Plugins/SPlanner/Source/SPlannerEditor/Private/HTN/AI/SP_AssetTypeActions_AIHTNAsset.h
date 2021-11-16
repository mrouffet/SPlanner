// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#include <Base/AI/SP_AssetTypeActions_AIAsset.h>
#include <Toolkits/IToolkitHost.h>


class FSP_AssetTypeActions_AIHTNAsset : public FSP_AssetTypeActions_AIAsset
{
public:
	FText GetName() const override;
	FColor GetTypeColor() const override;
	UClass* GetSupportedClass() const override;
};
