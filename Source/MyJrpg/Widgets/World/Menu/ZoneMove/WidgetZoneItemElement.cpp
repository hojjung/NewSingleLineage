#include "WidgetZoneItemElement.h"

#include "MyJrpg/MyLib.h"

void UWidgetZoneItemElement::SetZone(const FDropRewardItem& data)
{
	m_ID = data.m_Item.RowName;
	
	m_ItemRow = data.m_Item.GetRow<FItemDataRow>("");

	m_nCntDrop = data.m_nExpectDropCount;

	check(m_ItemRow);

	m_ElementBase->SetIcon(m_ItemRow->m_Icon);

	m_ElementBase->SetGlowColor(m_ItemRow->m_ColorHandle);

	m_ElementBase->SetHoldable(true);
	
	m_ElementBase->SetFocusable(false);

	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetZoneItemElement::OnHoldComplete);
}

int UWidgetZoneItemElement::GetSortValue() const
{
	return m_nCntDrop;
}

void UWidgetZoneItemElement::OnHoldComplete()
{
	if (!m_ID.IsNone())
		UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::QuestReward,m_ID,nullptr);	
}