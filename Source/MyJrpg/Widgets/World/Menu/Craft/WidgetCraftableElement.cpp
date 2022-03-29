#include "WidgetCraftableElement.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftableElement::SetCraftable(int index)
{
	m_nIndex = index;
	
	auto& Row = UMyGameInstance::Get->m_CraftManager->GetAllCraftData()[m_nIndex];
	
	m_ItemElement->SetIcon(Row->m_ItemIcon);

	m_ItemElement->SetGlowColor(Row->m_ColorHandle);

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

int UWidgetCraftableElement::GetIndex() const
{
	return m_nIndex;
}

void UWidgetCraftableElement::OnHoldComplete()
{
	FName ID = UMyGameInstance::Get->m_CraftManager->GetItemKey(GetIndex());
	
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Craft,ID,nullptr);
}

void UWidgetCraftableElement::OnClicked()
{
	m_OnClicked.Broadcast(this,m_nIndex);
}


