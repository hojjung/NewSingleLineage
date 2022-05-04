// All right Reserve 2021 HereticByte

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/QuestData.h"
#include "MyJrpg/Managers/EnchantManager.h"
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
	
	FItemSpec(FName id, int lvStack, int dur)
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

protected:
	bool GetEmptyIndex(int& out) const;

	bool AddItemStack(int index, int& lvCnt, FName id, int maxStack);

	bool RemoveItemStack(int index, int& stackCnt);

	void ClearItem(int index);
	
public:
	void Init(int size);
	
	int GetInvenSize() const;
	
	void UpdateInventory();
	
	bool AddItem(FName itemID, int lvCnt);

	bool RemoveItem(FName itemID, int lvCnt);

	FORCEINLINE const TArray<FItemSpec>& GetItems() const
	{
		return m_AryTotalItems;
	}
};