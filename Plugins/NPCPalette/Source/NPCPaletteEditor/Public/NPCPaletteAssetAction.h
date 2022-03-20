// My First Hack n Slash

#pragma once

#include "AssetTypeActions_Base.h"
#include "CoreMinimal.h"

/**
 * 
 */
class  FNPCPaletteAssetAction: public FAssetTypeActions_Base
{
public:
	FNPCPaletteAssetAction(uint32 InAssetCategory);

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	// End of IAssetTypeActions interface

private:
	uint32 MyAssetCategory;
};
