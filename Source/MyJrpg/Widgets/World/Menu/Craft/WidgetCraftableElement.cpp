#include "WidgetCraftableElement.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftableElement::SetCraftable(const FCraftable& id, int index)
{
	m_PtrCraftData =  &id;

	m_ItemElement->SetIcon(m_PtrCraftData->GetEntityRow()->m_Icon);

	m_ItemElement->SetIndex(index);
	
	m_ItemElement->m_OnFocus.AddUObject(this,&UWidgetCraftableElement::OnClicked);

	m_ItemElement->m_OnHold.AddUObject(this,&UWidgetCraftableElement::OnHoldComplete);

	m_ItemElement->SetTextStackLv(GetCraftItem().GetEntityRow()->m_ShowingName);

	m_ItemElement->SetHoldable(true);

	m_ItemElement->SetFocusable(true);

	m_ItemElement->SetDragable(false);
}

const FCraftable& UWidgetCraftableElement::GetCraftItem() const
{
	return *m_PtrCraftData;
}

void UWidgetCraftableElement::OnHoldComplete(UWidgetBaseElement*)
{
	
}

void UWidgetCraftableElement::OnClicked(UWidgetBaseElement*)
{
	m_ItemElement->SetMyUnFocus();
	
	m_OnClicked.Broadcast(*m_PtrCraftData,m_ItemElement->GetIndex());
}


