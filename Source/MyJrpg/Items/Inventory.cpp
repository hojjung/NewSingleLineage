#include "Inventory.h"
#include "Item_Exe/ItemExecuteBase.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
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

bool UInventory::AddItem(FItemSpec&& addItem, bool newEquipItem)
{
	return AddItem(addItem,newEquipItem);
}

int UInventory::GetInvenSize() const
{
	return m_nInvenMaxSize;
}

bool UInventory::AddItem(FItemSpec& addItem, bool newEquipItem)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(addItem.m_ID);
	
	bool IsEquip = UMyLib::IsEquip(ItemData);
	
	int MaxStack = ItemData.m_nMaxStack;

	int Iter = 0;

	if (IsEquip)
	{
		for (const FItemSpec& Item : m_AryTotalItems)
		{
			if (Item.m_ID.IsNone())
			{
				if(newEquipItem)
				{
					addItem.m_nDurability = ItemData.m_nDurability;
				}
				AddSlot(Iter,addItem);
				AddItemKey(ItemData,addItem.m_ID,Iter);
				UpdateInventory();
				return true;
			}
			Iter++;
		}
	}
	else
	{	//이미 있을수도있으니까 찾아서 넣어줌
		TSet<int>* FoundSet = m_MapItemKeyCount.Find(addItem.m_ID);
		if(FoundSet)
		{
			for(int Index : *FoundSet)
			{
				int RemainStack = MaxStack - m_AryTotalItems[Index].m_nLvStack;//넣을수있는거

				if(RemainStack > 0)//넣을수있는공간이 0보다 크다면
				{
					if(RemainStack > addItem.m_nLvStack)//남은공간 3, 넣을것 1개
					{
						m_AryTotalItems[Index].m_nLvStack += addItem.m_nLvStack;
						UpdateInventory();
						return true;//다넣었음
					}
					else//남은공간 1개 넣을것 3개,남은거 3개 넣을것 3개
					{
						m_AryTotalItems[Index].m_nLvStack += RemainStack;
						addItem.m_nLvStack -= RemainStack;
						if(addItem.m_nLvStack <= 0)
						{
							UpdateInventory();
							return true;
						}
					}
				}
			}
		}
		
		for (const FItemSpec& Item : m_AryTotalItems)
		{
			if (Item.m_ID == addItem.m_ID || Item.m_ID.IsNone())
			{
				AddItemStack(ItemData, Iter, addItem.m_nLvStack, addItem.m_ID, MaxStack);
				if(addItem.m_nLvStack <= 0)
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

void UInventory::AddSlot(int index, FItemSpec addItem)
{
	m_AryTotalItems[index] = addItem;
}

bool UInventory::HasSpace(FItemSpec& addItem)
{
	if(UMyLib::IsEquip(addItem.m_ID))
	{
		return EmptySlotCount() > 0;
	}
	TSet<int>* FoundSet = m_MapItemKeyCount.Find(addItem.m_ID);

	if(FoundSet)
	{
		int MaxStack = UMyLib::GetItemData(addItem.m_ID).m_nMaxStack;
		for(int Index : *FoundSet)
		{
			int RemainStack = MaxStack - m_AryTotalItems[Index].m_nLvStack;

			addItem.m_nLvStack -= RemainStack;

			if(addItem.m_nLvStack <= 0)
			{
				return true;
			}
		}

		return false;
	}

	return EmptySlotCount() > 0;
}

int UInventory::GetAvailableCount(FName itemID)
{
	if(UMyLib::IsEquip(itemID))
	{
		return EmptySlotCount();
	}
	TSet<int>* FoundSet = m_MapItemKeyCount.Find(itemID);

	if(FoundSet)
	{
		int Sum = 0;
		
		int MaxStack = UMyLib::GetItemData(itemID).m_nMaxStack;
		
		for(int Index : *FoundSet)
		{
			Sum += MaxStack - m_AryTotalItems[Index].m_nLvStack;
		}
		return Sum;
	}

	return EmptySlotCount();
}

void UInventory::AddItemStack(const FItemDataRow& itemData, int index, int& lvCnt, FName id, int maxStack)// = FItemSpec(id,0,0);
{
	if(m_AryTotalItems[index].m_ID.IsNone())
	{
		m_AryTotalItems[index] = FItemSpec(id,0,0);
	}
	int& ItemSpecFound = m_AryTotalItems[index].m_nLvStack;

	int AvailableCnt = maxStack - ItemSpecFound;
	
	if(AvailableCnt<=0)
	{
		return ;
	}
	if (AvailableCnt >= lvCnt)//10, 5
	{
		ItemSpecFound += lvCnt;
		lvCnt = 0;
		AddItemKey(itemData, id,index);
		return ;
	}
	ItemSpecFound += AvailableCnt;
	lvCnt -= AvailableCnt;
	AddItemKey(itemData, id,index);
}

void UInventory::RemoveItemStack(const FItemDataRow& itemData, int index, int& stackCnt)
{
	int& CrntStack = m_AryTotalItems[index].m_nLvStack;
	
	if(CrntStack <= stackCnt)// 3 5,라면,3은 클리어 당한다ㅡ 하지만 2만큼 다른곳에서 빼야함
	{
		stackCnt -= CrntStack;
		RemoveItemKey(itemData, m_AryTotalItems[index].m_ID,index);
		ClearSlot(index);
		return;
	}//5 3

	CrntStack -= stackCnt;
	stackCnt = 0;
	return;//제거 종료,아이템을 비우는게 목적이 아니라 차감이 목적,칸과 상관이 없다.
}

void UInventory::ClearSlot(int index)
{
	m_AryTotalItems[index].m_ID = NAME_None;
	m_AryTotalItems[index].m_nLvStack = 0;
	m_AryTotalItems[index].m_nDurability = 0;
}

void UInventory::RegisterQuickItemExe(const FItemDataRow& itemData)
{
	if(!itemData.m_ClassExeItem->IsValidLowLevel())
	{
		return;
	}
	UMyGameInstance::Get->m_QuickManager->RegisterItem(itemData.m_ClassExeItem);
}

void UInventory::UnregisterQuickItemExe(const FItemDataRow& itemData)
{
	if(!itemData.m_ClassExeItem->IsValidLowLevel())
	{
		return;
	}
	UMyGameInstance::Get->m_QuickManager->UnregisterItem(itemData.m_ClassExeItem);
}

void UInventory::AddItemKey(const FItemDataRow& itemData,FName id, int index)
{
	TSet<int>* FoundIndexSets = m_MapItemKeyCount.Find(id);
	if(FoundIndexSets)
	{
		FoundIndexSets->Add(index);//중복거르기 알아서
		return;
	}
	TSet<int> NewIndexSet;
	NewIndexSet.Add(index);
	m_MapItemKeyCount.Emplace(id,NewIndexSet);
	RegisterQuickItemExe(itemData);
}

void UInventory::RemoveItemKey(const FItemDataRow& itemData,FName id, int index)//인덱스가 존재하는 칸을 없애는거
{
	TSet<int>* FoundIndexSets = m_MapItemKeyCount.Find(id);
	
	FoundIndexSets->Remove(index);

	if(FoundIndexSets->Num() < 1)
	{
		m_MapItemKeyCount.Remove(id);
		UnregisterQuickItemExe(itemData);
	}
}

void UInventory::RemoveItemKey(FName id, int index)
{
	const FItemDataRow& ItemData = UMyLib::GetItemData(id);

	RemoveItemKey(ItemData, id, index);
}

int UInventory::EmptySlotCount()
{
	return m_nInvenMaxSize - GetUsingSlotCount();
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
				RemoveItemKey(ItemData,itemID,Iter);
				ClearSlot(Iter);
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
				RemoveItemStack(ItemData, Iter, lvCnt);
				
				if(lvCnt <= 0)
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

bool UInventory::RemoveItem(const FItemSpec& target, int am)
{
	TSet<int>* IndexSets = m_MapItemKeyCount.Find(target.m_ID);
	if(!IndexSets)
	{
		return false;
	}
	for(int Index : *IndexSets)
	{
		if(&m_AryTotalItems[Index] == &target)
		{
			m_AryTotalItems[Index].m_nLvStack-=am;
			if(m_AryTotalItems[Index].m_nLvStack < 1)
			{
				RemoveItemKey(target.m_ID,Index);
				ClearSlot(Index);
			}
			UpdateInventory();
			return true;
		}
	}
	return false;
}

bool UInventory::RemoveItem(const FItemSpec& target)
{
	TSet<int>* IndexSets = m_MapItemKeyCount.Find(target.m_ID);
	if(!IndexSets)
	{
		return false;
	}
	for(int Index : *IndexSets)
	{
		if(&m_AryTotalItems[Index] == &target)
		{
			RemoveItemKey(target.m_ID,Index);
			ClearSlot(Index);
			UpdateInventory();
			return true;
		}
	}
	return false;
}

void UInventory::RemoveItem(int index, int lvCnt)
{
	m_AryTotalItems[index].m_nLvStack -= lvCnt;
	
	if(m_AryTotalItems[index].m_nLvStack <= 0)
	{
		const FItemDataRow& ItemData = UMyLib::GetItemData(m_AryTotalItems[index].m_ID);
		RemoveItemKey(ItemData,m_AryTotalItems[index].m_ID,index);
		ClearSlot(index);
	}
	UpdateInventory();
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

FItemSpec* UInventory::FindItem(FName itemID, int stlv)
{
	TSet<int>* IndexSets = m_MapItemKeyCount.Find(itemID);
	if(!IndexSets)
	{
		return nullptr;
	}
	bool IsEquip = UMyLib::IsEquip(itemID);
	if(IsEquip)
	{
		for(int Index : *IndexSets)
		{
			if(stlv == -1 || GetItemRef(Index).m_nLvStack == stlv)
			{
				return &GetItemRef(Index);
			}
		}
		return nullptr;
	}
	int Sum = 0;
	for(int Index : (*IndexSets))
	{
		Sum += GetStLv(Index);
		if(Sum >= stlv)
		{
			return &GetItemRef(Index);
		}
	}
	return nullptr;
}

bool UInventory::ReduceDurability(const FItemSpec& item, int dur)
{
	TSet<int>* IndexSets = m_MapItemKeyCount.Find(item.m_ID);
	if(!IndexSets)
	{
		return false;
	}
	for(int Index : *IndexSets)
	{
		if(&m_AryTotalItems[Index] == &item)
		{
			ReduceDurability(Index, dur);
			return true;
		}
	}
	return false;
}

void UInventory::ReduceDurability(int index, int dur)//Equip은따로있는데?
{
	m_AryTotalItems[index].m_nDurability -= dur;
	if(m_AryTotalItems[index].m_nDurability < 1)
	{
		ClearSlot(index);
	}
	m_OnInvenChanged.Broadcast();
	UMyGameInstance::Get->m_EquipManager->UpdateDur();
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

	ClearSlot(myIndex);
	
	return true;
}

void UInventory::OnDropItem(int myIndex, UInventory* other, int other_index)
{
	FItemSpec OtherItem = other->GetItemConstRef(other_index);

	FItemSpec MyItem = GetItemConstRef(myIndex);

	const FItemDataRow& OtherItemData = UMyLib::GetItemData(OtherItem.m_ID);

	if (MyItem.m_ID.IsNone()) //빈슬롯이면 그냥 진행
	{
		AddSlot(myIndex, OtherItem);
		AddItemKey(OtherItemData,OtherItem.m_ID,myIndex);
		other->ClearSlot(other_index);
		other->RemoveItemKey(OtherItemData,OtherItem.m_ID,other_index);
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

			other->ClearSlot(other_index);
			other->RemoveItemKey(OtherItemData,OtherItem.m_ID,other_index);
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
		AddSlot(myIndex, OtherItem);
		AddItemKey(OtherItemData,OtherItem.m_ID,myIndex);
		other->RemoveItemKey(OtherItemData,OtherItem.m_ID,other_index);
		
		other->AddSlot(other_index, MyItem);
		other->AddItemKey(MyItemData, MyItem.m_ID, other_index);
		RemoveItemKey(MyItemData,MyItem.m_ID,myIndex);
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

UInventory::FOnInvenChanged& UInventory::GetOnInvenChanged()
{
	return m_OnInvenChanged;
}

int UInventory::GetItemCount(FName id, int stlv)
{
	int Sum = 0;
	TSet<int>* IndexSets = m_MapItemKeyCount.Find(id);
	if(!IndexSets)
	{
		return 0;
	}
	bool IsEquip = UMyLib::IsEquip(id);
	if(IsEquip)
	{
		for(int Index : (*IndexSets))
		{
			if(GetStLv(Index) == stlv)
			{
				Sum	++;
			}
		}
		return Sum;
	}
	for(int Index : (*IndexSets))
	{
		Sum += GetStLv(Index);
	}
	return Sum;
}
