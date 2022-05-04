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

bool UInventory::AddItem(FName itemID, int lvCnt)//스택을 채우냐,칸수를 늘리냐,장비템의 경우 언제나 칸수이다. 스텍템의 경우,20개담을때, 100개들어오면 5칸 생겨야함
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(itemID);
	
	bool IsEquip = UMyLib::IsEquip(ItemData);
	
	int AddStack = IsEquip ? 1 : lvCnt;

	int MaxStack = UMyLib::GetItemData(itemID).m_nMaxStack;

	int Iter = 0;

	for (auto& ItemMap : m_AryTotalItems)
	{
		if (ItemMap.m_ID == itemID || ItemMap.m_ID.IsNone())
		{
			if (AddItemStack(Iter, AddStack, itemID, MaxStack))
			{
				UpdateInventory();
				return true;
			}
		}
		Iter++;
	}

	UpdateInventory();
	return false;
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
		lvCnt = 0;
		return true;
	}//3,5
	
	ItemSpecFound += AvailableCnt;

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
		return false;
	}//5 3

	CrntStack -= stackCnt;
	
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
