// All right Reserve 2021 HereticByte

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "Inventory.generated.h"

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

	TMap<FName, int> m_MapItems;

protected:
	bool GetEmptyIndex(int& out) const;

	bool AddItemStack(int index, int& lvCnt, FName id, int maxStack);

	bool RemoveItemStack(int index, int& stackCnt);

	void ClearItem(int index);
	
	void AddMapItem(FName id, int cnt);

	void RemoveMapItem(FName id, int cnt);
	
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
	
	void Init(int size);
	
	int GetInvenSize() const;
	
	void UpdateInventory();

	bool AddItem(FItemSpec addItem);

	void AddItem(int index, FItemSpec addItem);
	
	bool RemoveItem(FName itemID, int lvCnt);

	void RemoveItem(int index);

	int GetUsingSlotCount() const;
	
	bool FindItem(FName itemID);

	bool MoveItem(int myIndex, UInventory* targetInvenToAdd);
	
	void OnDropItem(int myIndex, UInventory* other, int other_index);

	void SetStLv(int index, int stLv);

	int GetStLv(int index);
};

