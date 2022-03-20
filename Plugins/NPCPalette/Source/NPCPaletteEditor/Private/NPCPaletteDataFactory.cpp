#include "NPCPaletteDataFactory.h"

#include "NPCPaletteAssetAction.h"
#include "NPCPaletteDataAsset.h"
#include "NPCPaletteEditor.h"


UNPCPaletteDataFactory::UNPCPaletteDataFactory()
{
	SupportedClass = UNPCPaletteDataAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

uint32 UNPCPaletteDataFactory::GetMenuCategories() const
{
	return FNPCPaletteEditorModule::GameAssetCategory;
}

UObject* UNPCPaletteDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                  UObject* Context, FFeedbackContext* Warn)
{
	UNPCPaletteDataAsset* CreatedAsset = NewObject<UNPCPaletteDataAsset>(InParent, InClass, InName, Flags);
	
	return CreatedAsset;
}

bool UNPCPaletteDataFactory::ShouldShowInNewMenu() const
{
	return true;
}
