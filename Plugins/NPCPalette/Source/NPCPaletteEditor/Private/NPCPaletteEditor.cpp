// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NPCPaletteEditor/Public/NPCPaletteEditor.h"

#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "DataTableEditorUtils.h"
#include "EditorModeManager.h"
#include "IAssetTools.h"
#include "NPCPaletteAssetAction.h"
#include "NPCPaletteDataFactory.h"
#include "NPCPaletteGizmoActor.h"
#include "NPCPaletteGizmoActorFactory.h"
#include "ActorFactories/ActorFactoryBoxVolume.h"

#include "Engine/BrushBuilder.h"
#include "Misc/ConfigCacheIni.h"
#include "NPCPaletteEditor/Public/NPCPaletteEditorEdMode.h"

#define LOCTEXT_NAMESPACE "FNPCPaletteEditorModule"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

class OnDataTableChanged :public INotifyOnChanged<UDataTable,FDataTableEditorUtils::EDataTableChangeInfo>//,public FDataTableEditorUtils::INotifyOnDataTableChanged
{
public:
	virtual ~OnDataTableChanged() 
	{
		Module = nullptr;
	}
	
	FNPCPaletteEditorModule* Module;
	
	virtual void PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo ChangedType)override
	{
		//Module->OnAllPlaceableAssetsChanged();
	}
	virtual void PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo ChangedType)override
	{
		if(!Changed->RowStruct->IsChildOf(FEntityRow::StaticStruct()))
		{
			return;
		}
		Module->AllPlaceableAssetsChanged.Broadcast();
	}
	
	virtual void SelectionChange(const UDataTable* DataTable, FName RowName)
	{
		
	}
};



TOptional<FLinearColor> GetBasicShapeColorOverride()
{
	// Get color for basic shapes.  It should appear like all the other basic types
	static TOptional<FLinearColor> BasicShapeColorOverride;

	if (!BasicShapeColorOverride.IsSet())
	{
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
		TSharedPtr<IAssetTypeActions> AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(
			UClass::StaticClass()).Pin();
		if (AssetTypeActions.IsValid())
		{
			BasicShapeColorOverride = TOptional<FLinearColor>(AssetTypeActions->GetTypeColor());
		}
	}
	return BasicShapeColorOverride;
}

uint32 FNPCPaletteEditorModule::GameAssetCategory;

void FNPCPaletteEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	GameAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("NPC_Palette")),
	                                                             LOCTEXT("NPCPaletteAssetCategory",
	                                                                     "NPC_Palette_Data"));

	AssetTools.RegisterAssetTypeActions(MakeShareable(new FNPCPaletteAssetAction(GameAssetCategory)));
	////////StartNewModule
	///
	TArray<FString> RecentlyPlacedAsStrings;
	GConfig->GetArray(TEXT("PlacementMode"), TEXT("RecentlyPlaced"), RecentlyPlacedAsStrings, GEditorPerProjectIni);

	//FString ActivePaletteName;
	//GConfig->GetString( TEXT( "PlacementMode" ), TEXT( "ActivePalette" ), ActivePaletteName, GEditorPerProjectIni );

	for (int Index = 0; Index < RecentlyPlacedAsStrings.Num(); Index++)
	{
		RecentlyPlaced.Add(FNPCPlacementInfo(RecentlyPlacedAsStrings[Index]));
	}

	FEditorModeRegistry::Get().RegisterMode<FNPCPaletteEdMode>(
		FNPCPaletteEdMode::EM_NPCPaletteEdModeId,
		NSLOCTEXT("PlacementMode", "DisplayName", "Place"),FSlateIcon(),true);
//		true, 0);
	//
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().OnAssetRemoved().AddRaw(this, &FNPCPaletteEditorModule::OnAssetRemoved);
	AssetRegistryModule.Get().OnAssetRenamed().AddRaw(this, &FNPCPaletteEditorModule::OnAssetRenamed);
	AssetRegistryModule.Get().OnAssetAdded().AddRaw(this, &FNPCPaletteEditorModule::OnAssetAdded);
	//

	m_OnDataTableChanged = MakeShared<OnDataTableChanged>();

	m_OnDataTableChanged->Module = this;
	
	FDataTableEditorUtils::FDataTableEditorManager::Get().AddListener(m_OnDataTableChanged.Get());

	//
	TOptional<FLinearColor> BasicShapeColorOverride = GetBasicShapeColorOverride();

		RegisterPlacementCategory(
			FNPCEntityCategoryInfo(
				NSLOCTEXT("PlacementMode", "AllClasses", "All Classes"),
				FNPCPalettePlacementCategories::AllClasses(),
				TEXT("PMAllClasses"),
				50
			)
		);
	
}

void FNPCPaletteEditorModule::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FNPCPaletteEdMode::EM_NPCPaletteEdModeId);
	
	FDataTableEditorUtils::FDataTableEditorManager::Get().RemoveListener(m_OnDataTableChanged.Get());

	m_OnDataTableChanged.Reset();
}

void FNPCPaletteEditorModule::PreUnloadCallback()
{
	FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>("AssetRegistry");
	if (AssetRegistryModule)
	{
		AssetRegistryModule->Get().OnAssetRemoved().RemoveAll(this);
		AssetRegistryModule->Get().OnAssetRenamed().RemoveAll(this);
		AssetRegistryModule->Get().OnAssetAdded().RemoveAll(this);
	}
}

void FNPCPaletteEditorModule::AddToRecentlyPlaced(const TArray<UObject*>& PlacedObjects, UActorFactory* FactoryUsed)
{
	FString FactoryPath;
	if (FactoryUsed != NULL)
	{
		FactoryPath = FactoryUsed->GetPathName();
	}

	TArray<UObject*> FilteredPlacedObjects;
	for (UObject* PlacedObject : PlacedObjects)
	{
		// Don't include null placed objects that just have factories.
		if (PlacedObject == NULL)
		{
			continue;
		}

		// Don't add brush builders to the recently placed.
		if (PlacedObject->IsA(UBrushBuilder::StaticClass()))
		{
			continue;
		}

		FilteredPlacedObjects.Add(PlacedObject);
	}

	// Don't change the recently placed if nothing passed the filter.
	if (FilteredPlacedObjects.Num() == 0)
	{
		return;
	}

	bool Changed = false;
	for (int Index = 0; Index < FilteredPlacedObjects.Num(); Index++)
	{
		Changed |= RecentlyPlaced.Remove(FNPCPlacementInfo(FilteredPlacedObjects[Index]->GetPathName(), FactoryPath)) >
			0;
	}

	for (int Index = 0; Index < FilteredPlacedObjects.Num(); Index++)
	{
		if (FilteredPlacedObjects[Index] != NULL)
		{
			RecentlyPlaced.Insert(FNPCPlacementInfo(FilteredPlacedObjects[Index]->GetPathName(), FactoryPath), 0);
			Changed = true;
		}
	}

	for (int Index = RecentlyPlaced.Num() - 1; Index >= 20; Index--)
	{
		RecentlyPlaced.RemoveAt(Index);
		Changed = true;
	}

	if (Changed)
	{
		TArray<FString> RecentlyPlacedAsStrings;
		for (int Index = 0; Index < RecentlyPlaced.Num(); Index++)
		{
			RecentlyPlacedAsStrings.Add(RecentlyPlaced[Index].ToString());
		}

		GConfig->SetArray(TEXT("PlacementMode"), TEXT("RecentlyPlaced"), RecentlyPlacedAsStrings, GEditorPerProjectIni);
		RecentlyPlacedChanged.Broadcast(RecentlyPlaced);
	}
}

void FNPCPaletteEditorModule::OnAssetRemoved(const FAssetData&)
{
	RecentlyPlacedChanged.Broadcast(RecentlyPlaced);
	AllPlaceableAssetsChanged.Broadcast();
}

void FNPCPaletteEditorModule::OnAssetRenamed(const FAssetData& AssetData, const FString& OldObjectPath)
{
	for (auto& RecentlyPlacedItem : RecentlyPlaced)
	{
		if (RecentlyPlacedItem.ObjectPath == OldObjectPath)
		{
			RecentlyPlacedItem.ObjectPath = AssetData.ObjectPath.ToString();
			break;
		}
	}

	RecentlyPlacedChanged.Broadcast(RecentlyPlaced);
	AllPlaceableAssetsChanged.Broadcast();
}

void FNPCPaletteEditorModule::OnAssetAdded(const FAssetData& AssetData)
{
	AllPlaceableAssetsChanged.Broadcast();
}

void FNPCPaletteEditorModule::AddToRecentlyPlaced(UObject* Asset, UActorFactory* FactoryUsed)
{
	TArray<UObject*> Assets;
	Assets.Add(Asset);
	AddToRecentlyPlaced(Assets, FactoryUsed);
}

bool FNPCPaletteEditorModule::RegisterPlacementCategory(const FNPCEntityCategoryInfo& Info)
{
	if (Categories.Contains(Info.UniqueHandle))
	{
		return false;
	}

	Categories.Add(Info.UniqueHandle, Info);
	return true;
}

void FNPCPaletteEditorModule::UnregisterPlacementCategory(FName Handle)
{
	Categories.Remove(Handle);
}

void FNPCPaletteEditorModule::GetSortedCategories(TArray<FNPCEntityCategoryInfo>& OutCategories) const
{
	TArray<FName> SortedNames;
	Categories.GenerateKeyArray(SortedNames);

	SortedNames.Sort([&](const FName& A, const FName& B)
	{
		return Categories[A].SortOrder < Categories[B].SortOrder;
	});

	OutCategories.Reset(Categories.Num());
	for (const FName& Name : SortedNames)
	{
		OutCategories.Add(Categories[Name]);
	}
}

TOptional<FNPCPalette_ModeID> FNPCPaletteEditorModule::RegisterPlaceableItem(FName CategoryName,
                                                                             const TSharedRef<FNPCPaletteItem>& InItem)
{
	FNPCEntityCategoryInfo* Category = Categories.Find(CategoryName);
	if (Category && !Category->CustomGenerator)
	{
		FNPCPalette_ModeID ID = CreateID(CategoryName);
		Category->Items.Add(ID.UniqueID, InItem);
		return ID;
	}
	return TOptional<FNPCPalette_ModeID>();
}

void FNPCPaletteEditorModule::UnregisterPlaceableItem(FNPCPalette_ModeID ID)
{
	FNPCEntityCategoryInfo* Category = Categories.Find(ID.Category);
	if (Category)
	{
		Category->Items.Remove(ID.UniqueID);
	}
}

void FNPCPaletteEditorModule::GetItemsForCategory(FName CategoryName,
                                                  TArray<TSharedPtr<FNPCPaletteItem>>& OutItems) const
{
	const FNPCEntityCategoryInfo* Category = Categories.Find(CategoryName);
	if (Category)
	{
		for (auto& Pair : Category->Items)
		{
			OutItems.Add(Pair.Value);
		}
	}
}

void FNPCPaletteEditorModule::GetFilteredItemsForCategory(FName CategoryName,
                                                          TArray<TSharedPtr<FNPCPaletteItem>>& OutItems,
                                                          TFunctionRef<bool(const TSharedPtr<FNPCPaletteItem>&)> Filter)
const
{
	const FNPCEntityCategoryInfo* Category = Categories.Find(CategoryName);
	if (Category)
	{
		for (auto& Pair : Category->Items)
		{
			if (Filter(Pair.Value))
			{
				OutItems.Add(Pair.Value);
			}
		}
	}
}

void FNPCPaletteEditorModule::RegenerateItemsForCategory(FName Category)
{
	if (Category == FNPCPalettePlacementCategories::RecentlyPlaced())
	{
		RefreshRecentlyPlaced();
	}
	else if (Category == FNPCPalettePlacementCategories::Volumes())
	{
		RefreshVolumes();
	}
	else if (Category == FNPCPalettePlacementCategories::AllClasses())
	{
		RefreshAllPlaceableClasses();
	}

	PlacementModeCategoryRefreshed.Broadcast(Category);
}



void FNPCPaletteEditorModule::RefreshRecentlyPlaced()
{
	FName CategoryName = FNPCPalettePlacementCategories::RecentlyPlaced();

	FNPCEntityCategoryInfo* Category = Categories.Find(CategoryName);
	if (!Category)
	{
		return;
	}

	Category->Items.Reset();


	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		TEXT("AssetRegistry"));

	for (const FNPCPlacementInfo& RecentlyPlacedItem : RecentlyPlaced)//최근것 등록 
	{
		// UObject* Asset = FindObject<UObject>(nullptr, *RecentlyPlacedItem.ObjectPath);
		//
		// // If asset is pending delete, it will not be marked as RF_Standalone, in which case we skip it
		// if (Asset != nullptr && Asset->HasAnyFlags(RF_Standalone))
		// {
		// 	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(*RecentlyPlacedItem.ObjectPath);
		//
		// 	if (AssetData.IsValid())
		// 	{
		// 		UActorFactory* Factory = FindObject<UActorFactory>(nullptr, *RecentlyPlacedItem.Factory);
		// 		TSharedPtr<FNPCPaletteItem> Ptr = MakeShareable(new FNPCPaletteItem(Factory, AssetData));
		// 		if (FString* FoundThumbnail = BasicShapeThumbnails.Find(RecentlyPlacedItem.ObjectPath))
		// 		{
		// 			Ptr->ClassThumbnailBrushOverride = FName(**FoundThumbnail);
		// 			Ptr->bAlwaysUseGenericThumbnail = true;
		// 			Ptr->AssetTypeColorOverride = GetBasicShapeColorOverride();
		// 		}
		// 		Category->Items.Add(CreateID(), Ptr);
		// 	}
		// }
	}
}

void FNPCPaletteEditorModule::RefreshVolumes()
{
	FName CategoryName = FNPCPalettePlacementCategories::Volumes();

	FNPCEntityCategoryInfo* Category = Categories.Find(CategoryName);
	if (!Category)
	{
		return;
	}

	Category->Items.Reset();

	// Add loaded classes
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		const UClass* Class = *ClassIt;

		if (!Class->HasAllClassFlags(CLASS_NotPlaceable) &&
			!Class->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists) &&
			Class->IsChildOf(AVolume::StaticClass()) &&
			Class->ClassGeneratedBy == nullptr)
		{
			//UActorFactory* Factory = GEditor->FindActorFactoryByClassForActorClass(UActorFactoryBoxVolume::StaticClass(), Class);
			//Category->Items.Add(CreateID(), MakeShareable(new FNPCPaletteItem(Factory, FAssetData(Class))));
		}
	}
}



void FNPCPaletteEditorModule::RefreshAllPlaceableClasses()
{
	FName CategoryName = FNPCPalettePlacementCategories::AllClasses();

	FNPCEntityCategoryInfo* Category = Categories.Find(CategoryName);
	
	if (!Category)
	{
		return;
	}

	Category->Items.Reset();

	//클릭하기전까지 데이터테이블이 안나옴
	//slate widgetcomp가 작동이안된다.
	//빌보드는 텍스처크기가 적용되어서 너무커짐
	for(TObjectIterator<UDataTable> classIt(EObjectFlags::RF_ClassDefaultObject,false,EInternalObjectFlags::None);classIt;++classIt)
	{
		if(!classIt->RowStruct->IsChildOf(FEntityRow::StaticStruct()))
		{
			continue;
		}
		
		TArray<FEntityRow*> AryEntityRow;
		
		classIt->GetAllRows<FEntityRow>("",AryEntityRow);

		TArray<FName> NameRow =  classIt->GetRowNames();//순서를 믿을수 없음

		for(int i=0; i< NameRow.Num();i++)
		{
			Category->Items.Add(CreateID(), MakeShareable(new FNPCPaletteItem(NameRow[i],AryEntityRow[i],*classIt,*UNPCPaletteGizmoActorFactory::StaticClass(), FAssetData(ANPCPaletteGizmoActor::StaticClass()))));
		}
	}
}

FGuid FNPCPaletteEditorModule::CreateID()
{
	return FGuid::NewGuid();
}

FNPCPalette_ModeID FNPCPaletteEditorModule::CreateID(FName InCategory)
{
	FNPCPalette_ModeID NewID;
	NewID.UniqueID = CreateID();
	NewID.Category = InCategory;
	return NewID;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNPCPaletteEditorModule, NPCPaletteEditor)
//IMPLEMENT_MODULE(FNPCPaletteEditorModule, NPCPaletteEditorModule)
