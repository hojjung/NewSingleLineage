#include "WidgetCraftableElement.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftableElement::OnClicked(UWidgetBaseElement*)
{
	m_ItemElement->SetMyUnFocus();
	
	m_OnClicked.Broadcast(m_ItemElement->GetIndex());
}

void UWidgetCraftableElement::SetCraftable(int index)
{
	m_ItemElement->SetIndex(index);

	const FCraftDataInfo& CraftData = UMyGameInstance::Get->m_CraftManager->GetAryCraftables()[m_ItemElement->GetIndex()];

	m_ItemElement->SetIcon(CraftData.m_ItemData->m_Icon);

	m_ItemElement->SetTextStackLv(CraftData.m_ItemData->m_ShowingName);
	
	m_ItemElement->SetIndex(index);
	
	m_ItemElement->m_OnFocus.AddUObject(this,&UWidgetCraftableElement::OnClicked);

	m_ItemElement->SetHoldable(false);

	m_ItemElement->SetFocusable(true);

	m_ItemElement->SetDragable(false);
}