// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "NPCPaletteDataAsset.h"
#include "ActorFactories/ActorFactory.h"

/**
 * 
 */


class FNPCPlacementInfo
{
public:
	FNPCPlacementInfo(const FString& InObjectPath, const FString& InFactory)
		: ObjectPath(InObjectPath)
		, Factory(InFactory)
	{
	}

	FNPCPlacementInfo(const FString& String)
	{
		if (!String.Split(FString(TEXT(";")), &ObjectPath, &Factory))
		{
			ObjectPath = String;
		}
	}

	bool operator==(const FNPCPlacementInfo& Other) const
	{
		return ObjectPath == Other.ObjectPath;
	}

	bool operator!=(const FNPCPlacementInfo& Other) const
	{
		return ObjectPath != Other.ObjectPath;
	}

	FString ToString() const
	{
		return ObjectPath + TEXT(";") + Factory;
	}


public:
	FString ObjectPath;
	
	FString Factory;
};

class FNPCPalette_ModeID
{
public:
	FNPCPalette_ModeID(const FNPCPalette_ModeID&) = default;
	FNPCPalette_ModeID& operator=(const FNPCPalette_ModeID&) = default;

	friend class FPlacementModeModule;

	FNPCPalette_ModeID() {}

	/** The category this item is held within */
	FName Category;

	/** Unique identifier (always universally unique across categories) */
	FGuid UniqueID;
};


struct FNPCPaletteItem//실제 엔피씨 객체 최소단위
{
	FNPCPaletteItem()
		: bAlwaysUseGenericThumbnail(false), m_SelectedEntityRow(nullptr), Factory(nullptr)
	{
	}

	FNPCPaletteItem(FName rowID,FEntityRow* rowSelected,UDataTable* parentTable,UClass& InAssetClass,
	                const FAssetData& InAssetData)
	:m_RowID(rowID),
	bAlwaysUseGenericThumbnail(false),
	
	AssetData(InAssetData)
	{
		Factory = (GEditor->FindActorFactoryByClass(&InAssetClass)),
		
		m_EntityParentTable = parentTable;
		
		m_SelectedEntityRow = rowSelected;
		
		bAlwaysUseGenericThumbnail = !m_SelectedEntityRow->m_Icon;

		if(m_SelectedEntityRow->m_ShowingName.IsEmpty())
		{
			DisplayName = FText::FromName(m_RowID);
		}
		else
		{
			DisplayName = m_SelectedEntityRow->m_ShowingName;	
		}

	}
	FText DisplayName;
	
	FName m_RowID;

	bool bAlwaysUseGenericThumbnail;
	
	/** Optional overridden color tint for the asset */
	TOptional<FLinearColor> AssetTypeColorOverride;

	/** Optional sort order (lowest first). Overrides default class name sorting. */
	TOptional<int32> SortOrder;

	FEntityRow* m_SelectedEntityRow;

	UActorFactory* Factory;

	UDataTable* m_EntityParentTable;

	FAssetData AssetData;
};


/**
 * Struct providing information for a user category of placement objects
 */
struct FNPCEntityCategoryInfo
{
	FNPCEntityCategoryInfo(FText InDisplayName, FName InHandle, FString InTag, int32 InSortOrder = 0, bool bInSortable = true)
		: DisplayName(InDisplayName), UniqueHandle(InHandle), SortOrder(InSortOrder), TagMetaData(MoveTemp(InTag)), bSortable(bInSortable)
	{
	}

	FText DisplayName;

	/** A unique name for this category */
	FName UniqueHandle;

	/** Sort order for the category tab (lowest first) */
	int32 SortOrder;

	/** Optional tag meta data for the tab widget */
	FString TagMetaData;

	/** Optional generator function used to construct this category's tab content. Called when the tab is activated. */
	TFunction<TSharedRef<SWidget>()> CustomGenerator;

	/** Whether the items in this category are automatically sortable by name. False if the items are already sorted. */
	bool bSortable;
	//

	TMap<FGuid, TSharedPtr<FNPCPaletteItem>> Items;
};
