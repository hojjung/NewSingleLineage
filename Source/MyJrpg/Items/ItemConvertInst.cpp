#include "ItemConvertInst.h"

#include "MyJrpg/MyLib.h"


bool UItemConvertInst::CheckRightItemEmpty()
{
	return m_RightItem.m_ID.IsNone();
}



void UItemConvertInst::SetConvertData(const FItemConvertRow& convertRow)
{
	m_ItemConvertRow = &convertRow;
}

const FItemConvertRow& UItemConvertInst::GetConvertRow() const
{
	return *m_ItemConvertRow;
}

const FItemSpec& UItemConvertInst::GetLeftItem() const
{
	return m_LeftItem;
}

const FItemSpec& UItemConvertInst::GetRightItem() const
{
	return m_RightItem;
}

const FItemSpec& UItemConvertInst::GetCostItem() const
{
	return m_CostItem;
}

bool UItemConvertInst::CheckLeftItemAvailable(const FItemSpec& item)
{
	for(const FItemConvertSet& ItemSet : m_ItemConvertRow->m_AryItems)
	{
		if(CheckLeftItemWithSet(ItemSet, item))
		{
			return true;
		}
	}
	return false;
}

bool UItemConvertInst::CheckCostItemAvailable(const FItemSpec& item)
{
	for(const FItemConvertSet& ItemSet : m_ItemConvertRow->m_AryItems)
	{
		if(CheckCostItemWithSet(ItemSet, item))
		{
			return true;
		}
	}
	return false;
}

void UItemConvertInst::SetLeftItem(UInventory* fromInven, int fromIndex)
{
	m_LeftItem = fromInven->GetItemRef(fromIndex);
	
	fromInven->ClearSlot(fromIndex);

	m_SelectedConvertSet = TryStartConvert(); 

	m_OnChanged.Broadcast();
}

void UItemConvertInst::SetCostItem(UInventory* fromInven, int fromIndex)
{
	m_CostItem = fromInven->GetItemRef(fromIndex);
	
	fromInven->ClearSlot(fromIndex);

	m_SelectedConvertSet = TryStartConvert();

	m_OnChanged.Broadcast();
}

void UItemConvertInst::StartConvert()//0시간일때,즉발의 경우 버튼을 눌러서 제작
{
	if(!m_SelectedConvertSet)
	{
		return;
	}

	m_RightItem = FItemSpec(m_SelectedConvertSet->m_RightItem.RowName,m_SelectedConvertSet->m_nRightItemStLv);

	if(UMyLib::IsEquip(m_LeftItem.m_ID))
	{
		m_LeftItem = FItemSpec();
	}
	else
	{
		m_LeftItem.m_nLvStack -= m_SelectedConvertSet->m_nLeftItemStLv;
		if(m_LeftItem.m_nLvStack <= 0)
		{
			m_LeftItem = FItemSpec();
		}
	}

	if(!m_CostItem.m_ID.IsNone())
	{
		if(UMyLib::IsEquip(m_CostItem.m_ID))
		{
			m_CostItem = FItemSpec();
		}
		else
		{
			m_CostItem.m_nLvStack -= m_SelectedConvertSet->m_nCostItemStLv;
			if(m_CostItem.m_nLvStack <= 0)
			{
				m_CostItem = FItemSpec();
			}
		}
	}
	
	m_SelectedConvertSet = TryStartConvert();
	
	m_OnChanged.Broadcast();
}

void UItemConvertInst::Tick(float delta_time)
{
	if(!m_SelectedConvertSet)
	{
		return;
	}

	m_fTimer -= delta_time;

	if(m_fTimer < 0)
	{
		StartConvert();
	}
}

bool UItemConvertInst::IsEmpty()
{
	return m_CostItem.m_ID.IsNone() && m_LeftItem.m_ID.IsNone() && m_RightItem.m_ID.IsNone();
}

void UItemConvertInst::OnDropItem(UInventory* get, int m_n_index)
{
	// FItemSpec OtherItem = other->GetItemConstRef(other_index);
	//
	// FItemSpec MyItem = GetItemConstRef(myIndex);
	//
	// const FItemDataRow& OtherItemData = UMyLib::GetItemData(OtherItem.m_ID);
	//
	// if (MyItem.m_ID.IsNone()) //빈슬롯이면 그냥 진행
	// 	{
	// 	AddSlot(myIndex, OtherItem);
	// 	AddItemKey(OtherItemData,OtherItem.m_ID,myIndex);
	// 	other->ClearSlot(other_index);
	// 	other->RemoveItemKey(OtherItemData,OtherItem.m_ID,other_index);
	// 	UpdateInventory();
	// 	other->UpdateInventory();
	// 	return;
	// 	}
	//
	// const FItemDataRow& MyItemData = UMyLib::GetItemData(MyItem.m_ID);
	//
	// if (!UMyLib::IsEquip(MyItemData) && MyItem.m_ID == OtherItem.m_ID)
	// {
	// 	int AvailableStack = MyItemData.m_nMaxStack - MyItem.m_nLvStack; //10,3,7,3
	//
	// 	int NewAddStack = OtherItem.m_nLvStack; //7, 3개만 넣어주고 3개 빼기
	//
	// 	if (AvailableStack >= NewAddStack)
	// 	{
	// 		int MyStack = MyItem.m_nLvStack + NewAddStack; //타겟에게 가능한 개수 추가, 드래그 삭제
	// 		
	// 		SetStLv(myIndex, MyStack);
	//
	// 		other->ClearSlot(other_index);
	// 		other->RemoveItemKey(OtherItemData,OtherItem.m_ID,other_index);
	// 	}
	// 	else
	// 	{
	// 		int MyStack = MyItem.m_nLvStack + AvailableStack; //타겟에게 최대 개수 추가, 드래그에게 차감
	//
	// 		int OtherStack = NewAddStack - AvailableStack;
	// 		
	// 		SetStLv(myIndex, MyStack);
	//
	// 		other->SetStLv(other_index, OtherStack);
	// 	}
	// }
	// else
	// {
	// 	AddSlot(myIndex, OtherItem);
	// 	AddItemKey(OtherItemData,OtherItem.m_ID,myIndex);
	// 	other->AddSlot(other_index, MyItem);
	// 	other->RemoveItemKey(OtherItemData,OtherItem.m_ID,other_index);
	// }
	// UpdateInventory();
	// other->UpdateInventory();
}

const FItemConvertSet* UItemConvertInst::TryStartConvert()
{
	m_fTimer = 0;
	if(m_LeftItem.m_ID.IsNone())
	{
		return nullptr;
	}
	for(const FItemConvertSet& ItemSet : m_ItemConvertRow->m_AryItems)
	{
		if(ItemSet.m_LeftItem.RowName == m_LeftItem.m_ID)
		{
			if(CheckLeftItemWithSet(ItemSet,m_LeftItem) && CheckCostItemWithSet(ItemSet,m_CostItem))
			{
				m_fTimer = ItemSet.m_fConvertingTime; 
				return &ItemSet;
			}
		}
	}
	return nullptr;
}

bool UItemConvertInst::CheckLeftItemWithSet(const FItemConvertSet& set, const FItemSpec& item)
{
	return set.m_LeftItem.RowName == item.m_ID && set.m_nLeftItemStLv <= item.m_nLvStack;
}

bool UItemConvertInst::CheckCostItemWithSet(const FItemConvertSet& set, const FItemSpec& item)
{
	return  set.m_CostItem.RowName.IsNone() || (set.m_CostItem.RowName == item.m_ID && set.m_nCostItemStLv <= item.m_nLvStack);
}
