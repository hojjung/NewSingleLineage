#include "Inventory.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UInventory::Init(int size)
{
	m_nInvenMaxSize = size;
	
	m_AryTotalItems.Init(FItemSpec(), m_nInvenMaxSize);
}

bool UInventory::GetEmptyIndex(int& out) const
{
	int Iter = 0;
	
	for(const auto& ItemMap : m_AryTotalItems)
	{
		if(ItemMap.m_ID == NAME_None)
		{
			out = Iter;
			return true;
		}
		Iter++;
	}
	out = INDEX_NONE;
	return false;
}

void UInventory::UpdateInventory()
{
	m_OnInvenChanged.Broadcast();
}

int UInventory::GetInvenSize() const
{
	return m_nInvenMaxSize;
}

bool UInventory::AddItem(FItemSpec addItem)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(addItem.m_ID);
	
	bool IsEquip = UMyLib::IsEquip(ItemData);
	
	int MaxStack = ItemData.m_nMaxStack;

	int Iter = 0;

	if (IsEquip)
	{
		for (auto& ItemMap : m_AryTotalItems)
		{
			if (ItemMap.m_ID.IsNone())
			{
				m_AryTotalItems[Iter] = addItem;
				UpdateInventory();
				return true;
			}
			Iter++;
		}
	}
	else
	{
		for (auto& ItemMap : m_AryTotalItems)
		{
			if (ItemMap.m_ID == addItem.m_ID || ItemMap.m_ID.IsNone())
			{
				if (AddItemStack(Iter, addItem.m_nLvStack, addItem.m_ID, MaxStack))
				{
					UpdateInventory();
					return true;
				}
			}
			Iter++;
		}
	}

	UpdateInventory();
	return false;
}

void UInventory::AddMapItem(FName id, int cnt)
{
	int* Count = m_MapItems.Find(id);
	if(Count)
	{
		(*Count) += cnt;
	}
	else
	{
		m_MapItems.Add(id, cnt);
	}
}

void UInventory::RemoveMapItem(FName id, int cnt)
{
	int& Cnt = m_MapItems[id];

	Cnt -= cnt;

	if(Cnt <= 0)
	{
		m_MapItems.Remove(id);
	}
}

bool UInventory::AddItemStack(int index, int& lvCnt, FName id, int maxStack)// = FItemSpec(id,0,0);
{
	if(m_AryTotalItems[index].m_ID.IsNone())
	{
		m_AryTotalItems[index] = FItemSpec(id,0,0);
	}
	int& ItemSpecFound = m_AryTotalItems[index].m_nLvStack;

	int AvailableCnt = maxStack - ItemSpecFound;
	
	if(AvailableCnt<=0)
	{
		return false;
	}
	if (AvailableCnt >= lvCnt)//10, 5
	{
		ItemSpecFound += lvCnt;
		AddMapItem(id, lvCnt);
		lvCnt = 0;
		return true;
	}
	ItemSpecFound += AvailableCnt;
	AddMapItem(id, AvailableCnt);
	lvCnt -= AvailableCnt;
	return false;
}

bool UInventory::RemoveItemStack(int index, int& stackCnt)
{
	int& CrntStack = m_AryTotalItems[index].m_nLvStack;

	
	if(CrntStack < stackCnt)// 3 5
	{
		stackCnt -= CrntStack;
		ClearItem(index);
		RemoveItem(m_AryTotalItems[index].m_ID, CrntStack);
		return false;
	}//5 3

	CrntStack -= stackCnt;
	RemoveItem(m_AryTotalItems[index].m_ID, stackCnt);
	return true;
}

void UInventory::ClearItem(int index)
{
	m_AryTotalItems[index].m_ID = NAME_None;
	m_AryTotalItems[index].m_nLvStack = 0;
	m_AryTotalItems[index].m_nDurability = 0;
}

bool UInventory::RemoveItem(FName itemID, int lvCnt)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(itemID);
	
	bool IsEquip = UMyLib::IsEquip(ItemData);
	
	int Iter = 0;
	
	if (IsEquip)
	{
		for (auto& ItemMap : m_AryTotalItems)
		{
			if (ItemMap.m_ID == itemID && ItemMap.m_nLvStack == lvCnt)//장비템의경우,닉네임과 레벨이 일치할때제거
			{
				ClearItem(Iter);
				UpdateInventory();
				return true;
			}
			Iter++;
		}
	}
	else
	{
		for (auto& ItemMap : m_AryTotalItems)
		{
			if (ItemMap.m_ID == itemID)
			{
				if (RemoveItemStack(Iter, lvCnt))
				{
					UpdateInventory();
					return true;
				}
			}
			Iter++;
		}
	}

	UpdateInventory();
	return false;
}

void UInventory::RemoveItem(int index)
{
	ClearItem(index);
}

int UInventory::GetUsingSlotCount() const
{
	int UsingSlotCnt = 0;
	
	for (auto& ItemMap : m_AryTotalItems)
	{
		if (!ItemMap.m_ID.IsNone())
		{
			UsingSlotCnt++;
		}
	}
	return UsingSlotCnt;
}

void UInventory::AddItemLevel(int index, int i)
{
	m_AryTotalItems[index].m_nLvStack += i;
}

bool UInventory::FindItem(FName itemID)
{
	return m_MapItems.Contains(itemID);
}
