#include "WidgetCraftableElement.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftableElement::SetCraftable(const FName& id)
{
	m_ItemID =  id;

	m_ItemData = UItemData::GetItemTable->FindRow<FItemDataRow>(m_ItemID, "");
	
	m_ItemElement->SetIcon(m_ItemData->m_ItemIcon);

	m_ItemElement->SetGlowColor(m_ItemData->m_ColorHandle);

	m_ItemElement->m_OnClick.AddUObject(this,&UWidgetCraftableElement::OnClicked);

	m_ItemElement->m_OnHold.AddUObject(this,&UWidgetCraftableElement::OnHoldComplete);

	m_ItemElement->SetHoldable(true);

	m_ItemElement->SetFocusable(true);
}

void UWidgetCraftableElement::SetUnfocus()
{
	m_ItemElement->SetMyUnFocus();
}

void UWidgetCraftableElement::SetMyFocus()
{
	m_ItemElement->SetMyFocus();
}

const FItemDataRow& UWidgetCraftableElement::GetItemDataRow() const
{
	return *m_ItemData;
}

void UWidgetCraftableElement::OnHoldComplete()
{
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Craft,m_ItemID,nullptr);
}

void UWidgetCraftableElement::OnClicked()
{
	m_OnClicked.Broadcast(this,m_ItemID);
}


