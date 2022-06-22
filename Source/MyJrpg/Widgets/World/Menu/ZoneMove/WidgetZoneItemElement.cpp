#include "WidgetZoneItemElement.h"

#include "MyJrpg/MyLib.h"

void UWidgetZoneItemElement::SetZone(const FName& itemID)
{
	m_ID = itemID;
	
	m_ItemRow = &UMyLib::GetItemData(m_ID);

	check(m_ItemRow);

	m_ElementBase->SetItemData(*m_ItemRow);

	m_ElementBase->SetHoldable(true);
	
	m_ElementBase->SetFocusable(false);

	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetZoneItemElement::OnHold);
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