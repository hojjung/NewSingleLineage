#include "WidgetZoneItemElement.h"

#include "MyJrpg/MyLib.h"

void UWidgetZoneItemElement::SetZone(const FDropRewardItem& data)
{
	m_ID = data.m_Item.RowName;
	
	m_ItemRow = data.m_Item.GetRow<FItemDataRow>("");

	m_nCntDrop = data.m_nExpectDropCount;

	check(m_ItemRow);

	m_ElementBase->SetItemData(*m_ItemRow);

	m_ElementBase->SetHoldable(true);
	
	m_ElementBase->SetFocusable(false);

	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetZoneItemElement::OnHold);
}

int UWidgetZoneItemElement::GetSortValue() const
{
	return m_nCntDrop;
}

void UWidgetZoneItemElement::OnHold(UWidgetBaseElement* ele)
{
	ele->SetMyUnFocus();

	if(UMyLib::GetPlayerCon())
	{
		UMyLib::GetCanvas()->OpenItemInfoData(*m_ItemRow);
	}
	else
	{
		UMyLib::GetMapCanvas()->OpenItemInfoData(*m_ItemRow);
	}
	
}