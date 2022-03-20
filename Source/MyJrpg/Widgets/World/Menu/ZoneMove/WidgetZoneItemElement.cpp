#include "WidgetZoneItemElement.h"

void UWidgetZoneItemElement::SetZone(const FDropRewardItem& data)
{
	m_ItemRow = data.m_Item.GetRow<FItemDataRow>("");

	check(m_ItemRow);

	m_ElementBase->SetIcon(m_ItemRow->m_ItemIcon);

	m_ElementBase->SetGlowColor(m_ItemRow->m_ColorHandle);
}
