#include "ItemConvertInst.h"

#include "MyJrpg/MyLib.h"

void UItemConvertInst::Init(int size)
{
	Super::Init(size);
}

void UItemConvertInst::SetConvertData(const FItemConvertRow& convertRow)
{
	m_ItemConvertRow = &convertRow;
	Init(3);
	m_OnInvenChanged.AddUObject(this, &UItemConvertInst::OnInvenChanged);
	OnInvenChanged();
}

void UItemConvertInst::OnInvenChanged()
{	
		
}	

bool UItemConvertInst::CheckRightItemEmpty()//뭐가있더라 하더라도, 현재 컨버팅 세트에 지장이 없다면 그대로 해도됨
{
	return GetRightItem().m_ID.IsNone();
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
	if(!m_SelectedConvertSet)
	{
		return;
	}

	m_fConvertTimer -= delta_time;

	if(m_fConvertTimer < 0)
	{
		//StartConvert();
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
