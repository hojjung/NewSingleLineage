#pragma once

#include "CoreMinimal.h"
#include "Editor.h"
#include "FNPCPlacementInfo.h"
#include "ActorFactories/ActorFactory.h"
#include "GameFramework/Volume.h"
#include "Modules/ModuleManager.h"


/**
 * Structure defining a placeable item in the placement mode panel
 */
/** Structure of built-in placement categories. Defined as functions to enable external use without linkage */
struct FNPCPalettePlacementCategories
{
	static FName RecentlyPlaced()	{ static FName Name("RecentlyPlaced");	return Name; }
	static FName Basic()			{ static FName Name("Basic");			return Name; }
	static FName Lights()			{ static FName Name("Lights");			return Name; }
	static FName Visual()			{ static FName Name("Visual");			return Name; }
	static FName Volumes()			{ static FName Name("Volumes");			return Name; }
	static FName AllClasses()		{ static FName Name("AllClasses");		return Name; }
};


class  INPCPaletteEditorModule: public IModuleInterface
{

public:
	static inline INPCPaletteEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked< INPCPaletteEditorModule >( TEXT("NPCPaletteEditor") );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(TEXT("NPCPaletteEditor"));
	}
	
	virtual bool AllowAutomaticGraphicsSwitching() { return false; }
	
	virtual bool AllowMultipleGPUs() { return false; }
	//
	
	/**
	 * Add the specified assets to the recently placed items list
	 */
	virtual void AddToRecentlyPlaced( const TArray< UObject* >& Assets, UActorFactory* FactoryUsed = NULL ) = 0;
	
	/**
	 * Add the specified asset to the recently placed items list
	 */
	virtual void AddToRecentlyPlaced( UObject* Asset, UActorFactory* FactoryUsed = NULL ) = 0;

	/**
	 * Get a copy of the recently placed items
	 */
	virtual const TArray< FNPCPlacementInfo >& GetRecentlyPlaced() const = 0;

	/**
	 * @return the event that is broadcast whenever a placement mode category is refreshed
	 */
	DECLARE_EVENT_OneParam( IPlacementMode, FOnPlacementModeCategoryRefreshed, FName /*CategoryName*/ );
	virtual FOnPlacementModeCategoryRefreshed& OnPlacementModeCategoryRefreshed() = 0;

	/**
	 * @return the event that is broadcast whenever the list of recently placed assets changes
	 */
	DECLARE_EVENT_OneParam( IPlacementMode, FOnRecentlyPlacedChanged, const TArray< FNPCPlacementInfo >& /*NewRecentlyPlaced*/ );
	virtual FOnRecentlyPlacedChanged& OnRecentlyPlacedChanged() = 0;

	/**
	 * @return the event that is broadcast whenever the list of all placeable assets changes
	 */
	DECLARE_EVENT( IPlacementMode, FOnAllPlaceableAssetsChanged );
	virtual FOnAllPlaceableAssetsChanged& OnAllPlaceableAssetsChanged() = 0;

	/**
	 * @return the event that is broadcast whenever a placement mode enters a placing session
	 */
	DECLARE_EVENT_OneParam( IPlacementMode, FOnStartedPlacingEvent, const TArray< FAssetData >& /*Assets*/ );
	virtual FOnStartedPlacingEvent& OnStartedPlacing() = 0;
	virtual void BroadcastStartedPlacing( const TArray< FAssetData >& Assets ) = 0;

	/**
	 * @return the event that is broadcast whenever a placement mode exits a placing session
	 */
	DECLARE_EVENT_OneParam( IPlacementMode, FOnStoppedPlacingEvent, bool /*bWasSuccessfullyPlaced*/ );
	virtual FOnStoppedPlacingEvent& OnStoppedPlacing() = 0;
	virtual void BroadcastStoppedPlacing( bool bWasSuccessfullyPlaced ) = 0;

public:

	/**
	 * Register a new category of placement items
	 *
	 * @param Info		Information pertaining to the category
	 * @return true on success, false on failure (probably if the category's unique handle is already in use)
	 */
	virtual bool RegisterPlacementCategory(const FNPCEntityCategoryInfo& Info) = 0;

	/**
	 * Retrieve an already registered category
	 *
	 * @param UniqueHandle	The unique handle of the category to retrieve
	 * @return Ptr to the category's information structure, or nullptr if it does not exit
	 */
	virtual const FNPCEntityCategoryInfo* GetRegisteredPlacementCategory(FName UniqueHandle) const = 0;

	/**
	 * Populate the specified array with all registered category information, sorted by SortOrder
	 *
	 * @param OutCategories	The array to populate with registered category inforamtion
	 */
	virtual void GetSortedCategories(TArray<FNPCEntityCategoryInfo>& OutCategories) const = 0;

	/**
	 * Unregister a previously registered category
	 *
	 * @param UniqueHandle	The unique handle of the category to unregister
	 */
	virtual void UnregisterPlacementCategory(FName Handle) = 0;

	/**
	 * Register a new placeable item for the specified category
	 *
	 * @param Item			The placeable item to register
	 * @param CategoryName	Unique handle to the category in which to place this item
	 * @return Optional unique identifier for the registered item, or empty on failure (if the category doesn't exist)
	 */
	virtual TOptional<FNPCPalette_ModeID> RegisterPlaceableItem(FName CategoryName, const TSharedRef<FNPCPaletteItem>& Item) = 0;

	/**
	 * Unregister a previously registered placeable item
	 *
	 * @param ID			Unique identifier for the item. Will have been obtained from a previous call to RegisterPlaceableItem
	 */
	virtual void UnregisterPlaceableItem(FNPCPalette_ModeID ID) = 0;

	/**
	 * Get all the items in a given category, unsorted
	 *
	 * @param Category		The unique handle of the category to get items for
	 * @param OutItems		Array to populate with the items in this category
	 */
	virtual void GetItemsForCategory(FName Category, TArray<TSharedPtr<FNPCPaletteItem>>& OutItems) const = 0;

	/**
	 * Get all the items in a given category, filtered by the specified predicate
	 *
	 * @param Category		The unique handle of the category to get items for
	 * @param OutItems		Array to populate with the items in this category
	 * @param Filter 		Filter predicate used to filter out items. Return true to pass the filter, false otherwise
	 */
	virtual void GetFilteredItemsForCategory(FName Category, TArray<TSharedPtr<FNPCPaletteItem>>& OutItems, TFunctionRef<bool(const TSharedPtr<FNPCPaletteItem>&)> Filter) const = 0;

	/**
	 * Instruct the category associated with the specified unique handle that it should regenerate its items
	 *
	 * @param Category		The unique handle of the category to get items for
	 */
	virtual void RegenerateItemsForCategory(FName Category) = 0;
};


