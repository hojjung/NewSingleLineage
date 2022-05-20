// All right Reserve 2021 HereticByte

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "Inventory.generated.h"

struct FItemDataRow;
/**
 * 
 */
USTRUCT()
struct FItemSpec
{
	GENERATED_BODY()

public:
	FItemSpec(): m_nLvStack(0), m_nDurability(0)
	{
		m_ID = NAME_None;
	}
	
	FItemSpec(FName id, int lvStack, int dur = 0)
	{
		m_ID = id;
		m_nLvStack = lvStack;
		m_nDurability = dur;
	}

public:
	FName m_ID;

	int m_nLvStack;

	int m_nDurability;
};

UCLASS()
class MYJRPG_API UInventory : public UObject
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE(FOnInvenChanged);

	FOnInvenChanged m_OnInvenChanged;
	
protected://
	int m_nInvenMaxSize;
	
	TArray<FItemSpec> m_AryTotalItems;

	TMap<FName, TSet<int>> m_MapItemKeyCount;

protected:
	bool GetEmptyIndex(int& out) const;

	void AddItemStack(const FItemDataRow& itemData, int index, int& lvCnt, FName id, int maxStack);

	void RemoveItemStack(const FItemDataRow& itemData, int index, int& stackCnt);
	
	void RegisterQuickItemExe(const FItemDataRow& itemData);

	void UnregisterQuickItemExe(const FItemDataRow& itemData);


	
public:
	FORCEINLINE const TArray<FItemSpec>& GetAryItems() const
	{
		return m_AryTotalItems;
	}
	
	FORCEINLINE const FItemSpec& GetItemConstRef(int index) const
	{
		return m_AryTotalItems[index];
	}

	FORCEINLINE FItemSpec& GetItemRef(int index)
	{
		return m_AryTotalItems[index];
	}
	
	virtual void Init(int size);
	
	int GetInvenSize() const;
	
	void UpdateInventory();

	bool AddItem(FItemSpec addItem, bool newEquipItem = false);

	void AddSlot(int index, FItemSpec addItem);
	
	void ClearSlot(int index);
	
	bool RemoveItem(FName itemID, int lvCnt);

	void RemoveItem(int index, int lvCnt);

	int GetUsingSlotCount() const;
	
	int FindItem(FName itemID);

	void ReduceDurability(int index, int dur = 1);

	bool MoveItem(int myIndex, UInventory* targetInvenToAdd);
	
	void OnDropItem(int myIndex, UInventory* other, int other_index);

	void SetStLv(int index, int stLv);

	int GetStLv(int index);

	FOnInvenChanged& OnInvenChanged();

	int GetItemCount(FName id);

	void AddItemKey(const FItemDataRow& itemData,FName id, int index);

	void RemoveItemKey(const FItemDataRow& itemData,FName id, int index);
};

