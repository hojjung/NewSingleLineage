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

void UInventory::AddItem(int index, FItemSpec addItem)
{
	m_AryTotalItems[index] = addItem;
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

bool UInventory::FindItem(FName itemID)
{
	return m_MapItems.Contains(itemID);
}

bool UInventory::MoveItem(int myIndex, UInventory* targetInvenToAdd)
{
	FItemSpec Item = GetItemConstRef(myIndex);
	
	if(Item.m_ID.IsNone())
	{
		return false;
	}

	if(!targetInvenToAdd-AddItem(Item))
	{
		return false;
	}

	RemoveItem(myIndex);
	
	return true;
}

void UInventory::OnDropItem(int myIndex, UInventory* other, int other_index)
{
	FItemSpec OtherItem = other->GetItemConstRef(other_index);

	FItemSpec MyItem = GetItemConstRef(myIndex);

	if (MyItem.m_ID.IsNone()) //빈슬롯이면 그냥 진행
	{
		AddItem(myIndex, OtherItem);
		other->RemoveItem(other_index);
		UpdateInventory();
		other->UpdateInventory();
		return;
	}

	const FItemDataRow& MyItemData = UMyLib::GetItemData(MyItem.m_ID);

	if (!UMyLib::IsEquip(MyItemData) && MyItem.m_ID == OtherItem.m_ID)
	{
		int AvailableStack = MyItemData.m_nMaxStack - MyItem.m_nLvStack; //10,3,7,3

		int NewAddStack = OtherItem.m_nLvStack; //7, 3개만 넣어주고 3개 빼기

		if (AvailableStack >= NewAddStack)
		{
			int MyStack = MyItem.m_nLvStack + NewAddStack; //타겟에게 가능한 개수 추가, 드래그 삭제
			
			SetStLv(myIndex, MyStack);

			other->RemoveItem(other_index);
		}
		else
		{
			int MyStack = MyItem.m_nLvStack + AvailableStack; //타겟에게 최대 개수 추가, 드래그에게 차감

			int OtherStack = NewAddStack - AvailableStack;
			
			SetStLv(myIndex, MyStack);

			other->SetStLv(other_index, OtherStack);
		}
	}
	else
	{
		AddItem(myIndex, OtherItem);
		other->AddItem(other_index, MyItem);
	}
	UpdateInventory();
	other->UpdateInventory();
}

void UInventory::SetStLv(int index, int stLv)
{
	m_AryTotalItems[index].m_nLvStack = stLv;
}

int UInventory::GetStLv(int index)
{
	return m_AryTotalItems[index].m_nLvStack;
}