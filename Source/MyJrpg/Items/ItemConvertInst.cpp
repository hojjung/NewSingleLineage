#include "ItemConvertInst.h"

#include "MyJrpg/MyLib.h"

void UItemConvertInst::Init(int size)
{
	Super::Init(size);
}

void UItemConvertInst::SetConvertData(const FItemConvertRow& convertRow)
{
	m_fConvertTimer = 0;
	m_fFireTimer = 0;
	m_fMaxConvertTimer = 0;
	m_fMaxFireTimer = 0;
	m_SelectedConvertSet = nullptr;
	m_ItemConvertRow = &convertRow;
	Init(EItemConvertIndex::Len);
	m_OnInvenChanged.AddUObject(this, &UItemConvertInst::OnInvenChanged);
	OnInvenChanged();
}

void UItemConvertInst::TryUseFuelToFire()
{
	if(!GetFuelItem().m_ID.IsNone() && m_fFireTimer <= 0)//연료가 들어왔고, 불이 안돌아간다면
	{
		m_fMaxFireTimer = m_ItemConvertRow->m_fFuelDuration;
		m_fFireTimer = m_fMaxFireTimer; 

		int& StackCnt = m_AryTotalItems[EItemConvertIndex::Fuel].m_nLvStack;
		if(--StackCnt <= 0)
		{
			ClearSlot(EItemConvertIndex::Fuel);
		}
	}
}

void UItemConvertInst::TrySelectItemConvertSet()
{
	if(!GetLeftItem().m_ID.IsNone())
	{
		for(const FItemConvertSet& SetIter : m_ItemConvertRow->m_AryItems)
		{
			if(SetIter.m_LeftItem.RowName == GetLeftItem().m_ID && SetIter.m_nLeftItemStLv <= GetLeftItem().m_nLvStack)
			{
				m_SelectedConvertSet = &SetIter;
				break;
			}
		}
	}
	else
	{
		m_SelectedConvertSet = nullptr;

		m_fMaxConvertTimer = 0;
	}
}

void UItemConvertInst::TryStartConvert()
{
	if(m_SelectedConvertSet)
	{
		if(m_ItemConvertRow->m_FuelItem.Num() > 0 && m_fMaxFireTimer <= 0)
		{
			return;//불이필요한데 불이 안켜짐
		}

		if(!GetRightItem().m_ID.IsNone())
		{
			if(m_SelectedConvertSet->m_RightItem.RowName != GetRightItem().m_ID)
			{
				return;//다른칸이 존재하면 안돌아감
			}
			
			int ResultCount = m_SelectedConvertSet->m_nRightItemStLv;

			int MaxStackCount = UMyLib::GetItemData(m_SelectedConvertSet->m_RightItem.RowName).m_nMaxStack;
			
			if((GetRightItem().m_nLvStack + ResultCount) > MaxStackCount)
			{
				return;//중첩 가능한 결과값이 존재는 하는데 스택칸수가 꽉찼음
			}
		}
		//이제 컨버팅 시작해도됨
		int& LeftStack = m_AryTotalItems[EItemConvertIndex::Left].m_nLvStack;
		if(--LeftStack <= 0)
		{
			ClearSlot(EItemConvertIndex::Left);
		}
		
		m_fMaxConvertTimer = m_SelectedConvertSet->m_fConvertingTime;

		m_fConvertTimer = 0;
	}
}

void UItemConvertInst::OnInvenChanged()
{	
	TryUseFuelToFire();

	TrySelectItemConvertSet();

	TryStartConvert();

	m_OnItemConvertInst.Broadcast();
}	

bool UItemConvertInst::CheckRightItemEmpty()//뭐가있더라 하더라도, 현재 컨버팅 세트에 지장이 없다면 그대로 해도됨
{
	return GetRightItem().m_ID.IsNone();
}

void UItemConvertInst::ReceiveRightItem()
{
	m_fConvertTimer = 0;
	
	if(m_AryTotalItems[EItemConvertIndex::Right].m_ID.IsNone())
	{
		AddSlot(EItemConvertIndex::Right,FItemSpec( m_SelectedConvertSet->m_RightItem.RowName,m_SelectedConvertSet->m_nRightItemStLv));
	}
	else
	{
		m_AryTotalItems[EItemConvertIndex::Right].m_nLvStack += m_SelectedConvertSet->m_nRightItemStLv;
	}
	
	OnInvenChanged();
}

const FItemConvertRow& UItemConvertInst::GetConvertRow() const
{
	return *m_ItemConvertRow;
}

const FItemSpec& UItemConvertInst::GetLeftItem() const
{
	return m_AryTotalItems[EItemConvertIndex::Left];
}

const FItemSpec& UItemConvertInst::GetRightItem() const
{
	return m_AryTotalItems[EItemConvertIndex::Right];
}

const FItemSpec& UItemConvertInst::GetFuelItem() const
{
	return m_AryTotalItems[EItemConvertIndex::Fuel];
}

const FItemSpec& UItemConvertInst::GetCostItem() const
{
	return m_AryTotalItems[EItemConvertIndex::Cost];
}

bool UItemConvertInst::CheckLeftItemWithSet(const FItemConvertSet& set, const FItemSpec& item)
{
	return set.m_LeftItem.RowName == item.m_ID && set.m_nLeftItemStLv <= item.m_nLvStack;
}

bool UItemConvertInst::CheckCostItemWithSet(const FItemConvertSet& set, const FItemSpec& item)
{
	return set.m_CostItem.RowName == item.m_ID && set.m_nCostItemStLv <= item.m_nLvStack;
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

void UItemConvertInst::Tick(float delta_time)
{
	// if(m_fMaxFireTimer <= 0)
	// {
	// 	return;
	// }
	m_fFireTimer -= delta_time;

	if(m_fMaxConvertTimer <= 0)
	{
		return;
	}
	m_fConvertTimer += delta_time;

	if(m_fConvertTimer >= m_fMaxConvertTimer)
	{
		ReceiveRightItem();
	}
}

bool UItemConvertInst::IsEmpty()
{
	return GetFuelItem().m_ID.IsNone() && GetLeftItem().m_ID.IsNone() && GetRightItem().m_ID.IsNone();
}

float UItemConvertInst::GetRemainTimePer()
{
	return m_fConvertTimer / m_fMaxConvertTimer;
}

float UItemConvertInst::GetFireRemainTimePer()
{
	return m_fFireTimer / m_fMaxFireTimer;
}

bool UItemConvertInst::CheckFuelItemAvailable(const FItemSpec& item)
{
	if(m_ItemConvertRow->m_FuelItem.Num() <= 0)
	{
		return false;//허용된 자원이 없으면 자원칸에 아무것도 들어갈수 없음
	}
	for(const FItemDataHandle& FuelItem : m_ItemConvertRow->m_FuelItem)
	{
		if(FuelItem.RowName == item.m_ID)
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
