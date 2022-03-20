// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "INPCPaletteEditorModule.h"
#include "DataTableEditorUtils.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Modules/ModuleManager.h"

static TOptional<FLinearColor> GetBasicShapeColorOverride();

class OnDataTableChanged;



class FNPCPaletteEditorModule : public INPCPaletteEditorModule
{
public:
	static uint32 GameAssetCategory;
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void PreUnloadCallback() override;

	virtual ~FNPCPaletteEditorModule() override {} ;
	

	DECLARE_DERIVED_EVENT(FNPCPaletteEditorModule, INPCPaletteEditorModule::FOnPlacementModeCategoryRefreshed, FOnPlacementModeCategoryRefreshed);
	virtual FOnPlacementModeCategoryRefreshed& OnPlacementModeCategoryRefreshed() override { return PlacementModeCategoryRefreshed; }

	DECLARE_DERIVED_EVENT(FNPCPaletteEditorModule, INPCPaletteEditorModule::FOnRecentlyPlacedChanged, FOnRecentlyPlacedChanged);
	virtual FOnRecentlyPlacedChanged& OnRecentlyPlacedChanged() override { return RecentlyPlacedChanged; }

	DECLARE_DERIVED_EVENT(FNPCPaletteEditorModule, INPCPaletteEditorModule::FOnAllPlaceableAssetsChanged, FOnAllPlaceableAssetsChanged);
	virtual FOnAllPlaceableAssetsChanged& OnAllPlaceableAssetsChanged() override { return AllPlaceableAssetsChanged; }

	/**
	 * Add the specified assets to the recently placed items list
	 */
	virtual void AddToRecentlyPlaced(const TArray< UObject* >& PlacedObjects, UActorFactory* FactoryUsed = NULL) override;

	void OnAssetRemoved(const FAssetData& /*InRemovedAssetData*/);

	void OnAssetRenamed(const FAssetData& AssetData, const FString& OldObjectPath);

	void OnAssetAdded(const FAssetData& AssetData);

	/**
	 * Add the specified asset to the recently placed items list
	 */
	virtual void AddToRecentlyPlaced(UObject* Asset, UActorFactory* FactoryUsed = NULL) override;

	/**
	 * Get a copy of the recently placed items list
	 */
	virtual const TArray< FNPCPlacementInfo >& GetRecentlyPlaced() const override
	{
		return RecentlyPlaced;
	}

	/** @return the event that is broadcast whenever the placement mode enters a placing session */
	DECLARE_DERIVED_EVENT(FPlacementModeModule, INPCPaletteEditorModule::FOnStartedPlacingEvent, FOnStartedPlacingEvent);
	virtual FOnStartedPlacingEvent& OnStartedPlacing() override
	{
		return StartedPlacingEvent;
	}
	virtual void BroadcastStartedPlacing(const TArray< FAssetData>& Assets) override
	{
		StartedPlacingEvent.Broadcast(Assets);
	}

	/** @return the event that is broadcast whenever the placement mode exits a placing session */
	DECLARE_DERIVED_EVENT(FPlacementModeModule, INPCPaletteEditorModule::FOnStoppedPlacingEvent, FOnStoppedPlacingEvent);
	virtual FOnStoppedPlacingEvent& OnStoppedPlacing() override
	{
		return StoppedPlacingEvent;
	}
	virtual void BroadcastStoppedPlacing(bool bWasSuccessfullyPlaced) override
	{
		StoppedPlacingEvent.Broadcast(bWasSuccessfullyPlaced);
	}

public:

	virtual bool RegisterPlacementCategory(const FNPCEntityCategoryInfo& Info) override;

	virtual const FNPCEntityCategoryInfo* GetRegisteredPlacementCategory(FName CategoryName) const override
	{
		return Categories.Find(CategoryName);
	}

	virtual void UnregisterPlacementCategory(FName Handle) override;

	virtual void GetSortedCategories(TArray<FNPCEntityCategoryInfo>& OutCategories) const override;

	virtual TOptional<FNPCPalette_ModeID> RegisterPlaceableItem(FName CategoryName, const TSharedRef<FNPCPaletteItem>& InItem) override;

	virtual void UnregisterPlaceableItem(FNPCPalette_ModeID ID) override;

	virtual void GetItemsForCategory(FName CategoryName, TArray<TSharedPtr<FNPCPaletteItem>>& OutItems) const override;

	virtual void GetFilteredItemsForCategory(FName CategoryName, TArray<TSharedPtr<FNPCPaletteItem>>& OutItems, TFunctionRef<bool(const TSharedPtr<FNPCPaletteItem>&)> Filter) const override;

	virtual void RegenerateItemsForCategory(FName Category) override;


private:
	void RefreshRecentlyPlaced();

	void RefreshVolumes();

	void RefreshAllPlaceableClasses();

	FGuid CreateID();

	FNPCPalette_ModeID CreateID(FName InCategory);

public:

	TMap<FName, FNPCEntityCategoryInfo> Categories;

	TArray< FNPCPlacementInfo >	RecentlyPlaced;
	FOnRecentlyPlacedChanged		RecentlyPlacedChanged;

	FOnAllPlaceableAssetsChanged	AllPlaceableAssetsChanged;
	FOnPlacementModeCategoryRefreshed PlacementModeCategoryRefreshed;

	FOnStartedPlacingEvent			StartedPlacingEvent;
	FOnStoppedPlacingEvent			StoppedPlacingEvent;

	TArray< TSharedPtr<FExtender> > ContentPaletteFiltersExtenders;
	TArray< TSharedPtr<FExtender> > PaletteExtenders;

	TMap<FString, FString> BasicShapeThumbnails;

	TSharedPtr<OnDataTableChanged> m_OnDataTableChanged;

};
