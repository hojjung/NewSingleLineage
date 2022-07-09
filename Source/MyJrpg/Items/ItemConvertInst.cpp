#include "ItemConvertInst.h"

#include "MyJrpg/MyLib.h"

void UItemConvertInst::SetConvertData(const FItemConvertRow& convertRow)
{
	m_bAllMaterialAvailable = false;
	m_bIsFireWorking = false;
	m_bIsConverting = false;
	m_fFireTimer = 0;
	m_fMaxConvertTimer = -1;
	m_fMaxFireTimer = 0;
	m_SelectedConvertSet = nullptr;
	m_ItemConvertRow = &convertRow;
	m_bIsNeedFire = m_ItemConvertRow->m_AryFuelItems.Num() > 0 && m_fMaxFireTimer <= 0;
	Init(EItemConvertIndex::Len, convertRow.m_TextConverterName);

	m_bHasAnyCostItem = false;
	for(const FItemConvertSet& Set :m_ItemConvertRow->m_AryItems)
	{
		if(!Set.m_CostItem.RowName.IsNone())
		{
			m_bHasAnyCostItem = true;
		}
	}
	
	m_OnInvenChanged.AddUObject(this, &UItemConvertInst::OnInvenChanged);
	OnInvenChanged();
}


void UItemConvertInst::EndFireWorks()
{
	m_bIsFireWorking = false;
	m_fFireTimer = 0;
	m_fMaxFireTimer = 0;
}

void UItemConvertInst::EndConverts()
{
	m_bIsConverting = false;
	m_fConvertTimer = 0;
	m_fMaxConvertTimer = 0;
}

void UItemConvertInst::Tick(float delta_time)
{
	if(m_bIsFireWorking)
	{
		m_fFireTimer -= delta_time;
		
		if(m_fFireTimer<=0)
		{
			EndFireWorks();
			TryUseFuelToFire();
			m_OnConvertChanged.Broadcast();
		}
	}

	if(m_bIsConverting)
	{
		if(m_bIsNeedFire && !m_bIsFireWorking)
		{
			return;//불이필요하면 불을 떼줘야함
		}
		m_fConvertTimer += delta_time;

		if(m_fConvertTimer >= m_fMaxConvertTimer)
		{
			EndConverts();
			ReceiveRightItem();
		}
	}
}

void UItemConvertInst::TryUseFuelToFire()
{
	if(!GetFuelItem().m_ID.IsNone() && !m_bIsFireWorking)//연료가 들어왔고, 불이 안돌아간다면
	{
		for(const FFuelData& FuelD : m_ItemConvertRow->m_AryFuelItems)
		{
			if(FuelD.m_FuelItem.RowName == GetFuelItem().m_ID)
			{
				m_fMaxFireTimer = FuelD.m_fFuelDuration;
				break;
			}
		}
		m_fFireTimer = m_fMaxFireTimer;
		RemoveItemStack(EItemConvertIndex::Fuel,1);
		m_bIsFireWorking = true;
	}
}

void UItemConvertInst::TrySelectItemConvertSet()
{
	if(!GetLeftItem().m_ID.IsNone())
	{
		for(const FItemConvertSet& SetIter : m_ItemConvertRow->m_AryItems)
		{
			if(SetIter.m_LeftItem.RowName == GetLeftItem().m_ID)
			{
				m_SelectedConvertSet = &SetIter;
				break;
			}
		}
	}
	else
	{
		m_SelectedConvertSet = nullptr;
		EndConverts();
	}
}

void UItemConvertInst::RemoveItemStack(EItemConvertIndex t, int amount)
{
	int& CostStack = m_AryTotalItems[t].m_nLvStack;
	CostStack-=amount;
	if(CostStack <= 0)
	{
		ClearSlot(t);
	}
}

void UItemConvertInst::TryStartConvert()
{
	if(m_SelectedConvertSet && !m_bIsConverting && m_bAllMaterialAvailable)
	{
		if(m_bIsNeedFire && !m_bIsFireWorking)
		{
			return;//불이필요한데 불이 안켜짐
		}
		//결과칸 비어있는지 확인
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
		m_fMaxConvertTimer = m_SelectedConvertSet->m_fConvertingTime;

		m_fConvertTimer = 0;

		m_bIsConverting = true;
	}
}

void UItemConvertInst::CheckLeftOrCostAvailable()
{
	if(m_SelectedConvertSet)
	{
		if(!m_SelectedConvertSet->m_CostItem.RowName.IsNone())
		{
			if(!CheckCostItemWithSet(*m_SelectedConvertSet,GetCostItem()))
			{
				EndConverts();
				m_bAllMaterialAvailable = false;
				return;
			}
		}
		if(!m_SelectedConvertSet->m_LeftItem.RowName.IsNone())
		{
			if(!CheckLeftItemWithSet(*m_SelectedConvertSet,GetLeftItem()))
			{
				EndConverts();
				m_bAllMaterialAvailable = false;
				return;
			}
		}

		m_bAllMaterialAvailable = true;
	}
}

void UItemConvertInst::OnInvenChanged()
{	
	TryUseFuelToFire();

	TrySelectItemConvertSet();
	
	CheckLeftOrCostAvailable();

	TryStartConvert();

	m_OnConvertChanged.Broadcast();
}	

bool UItemConvertInst::CheckRightItemEmpty()//뭐가있더라 하더라도, 현재 컨버팅 세트에 지장이 없다면 그대로 해도됨
{
	return GetRightItem().m_ID.IsNone();
}

void UItemConvertInst::ReceiveRightItem()
{
	RemoveItemStack(EItemConvertIndex::Left,m_SelectedConvertSet->m_nLeftItemStLv);
	
	RemoveItemStack(EItemConvertIndex::Cost,m_SelectedConvertSet->m_nCostItemStLv);
	
	if(m_AryTotalItems[EItemConvertIndex::Right].m_ID.IsNone())
	{
		const FName& RightItem = m_SelectedConvertSet->m_RightItem.RowName;
		
		AddSlot(EItemConvertIndex::Right,FItemSpec(RightItem, m_SelectedConvertSet->m_nRightItemStLv));

		AddItemKey(UMyLib::GetItemData(RightItem),RightItem,EItemConvertIndex::Right);
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
		if(ItemSet.m_LeftItem.RowName == item.m_ID)
		{
			return true;
		}
	}
	return false;
}


bool UItemConvertInst::IsEmpty()
{
	return GetFuelItem().m_ID.IsNone() && GetLeftItem().m_ID.IsNone() && GetRightItem().m_ID.IsNone() && !m_bIsConverting;
}

float UItemConvertInst::GetRemainTimePer()
{
	if(m_fMaxConvertTimer <= 0.f)
	{
		return 0.f;
	}
	return m_fConvertTimer / m_fMaxConvertTimer;
}

float UItemConvertInst::GetFireRemainTimePer()
{
	if(m_fMaxFireTimer <= 0.f)
	{
		return 0.f;
	}
	return m_fFireTimer / m_fMaxFireTimer;
}

float UItemConvertInst::GetRemainTime() const
{
	return m_fMaxConvertTimer - m_fConvertTimer;
}

bool UItemConvertInst::IsFireWorking()
{
	return m_bIsFireWorking;
}

bool UItemConvertInst::IsConvertWorking()
{
	return m_bIsConverting;
}

const FItemDataRow* UItemConvertInst::GetRightItemData()
{
	if(!m_SelectedConvertSet || !m_bAllMaterialAvailable)
	{
		return nullptr;
	}

	return m_SelectedConvertSet->m_RightItem.GetRow<FItemDataRow>("");
}

bool UItemConvertInst::HasAnyCostItem()
{
	return m_bHasAnyCostItem;
}

bool UItemConvertInst::HasAnyFuelItem()
{
	return m_ItemConvertRow->m_AryFuelItems.Num() > 0;
}

bool UItemConvertInst::CheckFuelItemAvailable(const FItemSpec& item)
{
	if(!m_bIsNeedFire)
	{
		return false;//허용된 자원이 없으면 자원칸에 아무것도 들어갈수 없음
	}
	for(const FFuelData& FuelItem : m_ItemConvertRow->m_AryFuelItems)
	{
		if(FuelItem.m_FuelItem.RowName == item.m_ID)
		{
			return true;
		}
	}
	return false;
}

bool UItemConvertInst::CheckCostItemAvailable(const FItemSpec& item)
{
	if(!m_SelectedConvertSet)
	{
		return false;
	}

	if(m_SelectedConvertSet->m_CostItem.RowName == item.m_ID)
	{
		return true;
	}
	
	return false;
}
