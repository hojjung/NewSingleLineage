#include "NPCPaletteAssetAction.h"

#include "NPCPaletteDataAsset.h"


#define LOCTEXT_NAMESPACE "NPCPaletteAssetTypeActions"

FNPCPaletteAssetAction::FNPCPaletteAssetAction(uint32 InAssetCategory)
: MyAssetCategory(InAssetCategory)
{
}

FText FNPCPaletteAssetAction::GetName() const
{
	return LOCTEXT("FNPCPaletteAssetTypeActionsName", "NPCPalette");
}

FColor FNPCPaletteAssetAction::GetTypeColor() const
{
	return FColor(255, 55, 220);
}

UClass* FNPCPaletteAssetAction::GetSupportedClass() const
{
	return UNPCPaletteDataAsset::StaticClass();
}

uint32 FNPCPaletteAssetAction::GetCategories()
{
	return MyAssetCategory; //affects filters
}

#undef LOCTEXT_NAMESPACE