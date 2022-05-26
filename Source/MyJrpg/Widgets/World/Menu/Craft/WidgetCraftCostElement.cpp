#include "WidgetCraftCostElement.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftCostElement::SetCraftCost(const FCraftItemCost& cost)
{
	m_CraftData = &cost;
	
	const FItemDataRow* CraftData = m_CraftData->m_ItemDataRowHandle.GetRow<FItemDataRow>("");
	
	m_ItemElement->SetIcon(CraftData->m_Icon);

	m_ItemElement->SetGlowColor(CraftData->m_ColorHandle);

	//m_ItemElement->m_OnHold.AddUObject(this,&UWidgetCraftCostElement::OnHoldComplete);

	m_ItemElement->SetHoldable(true);

	m_ItemElement->SetFocusable(false);

	m_ItemElement->SetDragable(false);

	UpdateCostAmount();
}

void UWidgetCraftCostElement::OnHoldComplete()
{
	FName ID = m_CraftData->m_ItemDataRowHandle.RowName;
	
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Craft,ID,nullptr);
}

void UWidgetCraftCostElement::UpdateCostAmount()
{
	FString FormatAmount;

	int Stack = UMyLib::GetPlayerInven()->GetItemCount(m_CraftData->m_ItemDataRowHandle.RowName,m_CraftData->m_nStackOrLevel);

	if(UMyLib::IsEquip(m_CraftData->m_ItemDataRowHandle.RowName))
	{
		FormatAmount = FString::Printf(TEXT("Lv.+%d : %d/1"),m_CraftData->m_nStackOrLevel, Stack);
	}
	else
	{
		FormatAmount = FString::Printf(TEXT("%d/%d"),Stack,m_CraftData->m_nStackOrLevel);
	}
	
	FText TextWant = FText::FromString(FormatAmount); 
	
	m_TextCostAmount->SetText(TextWant);
	
	if(Stack > 0)
	{
		m_TextCostAmount->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		m_TextCostAmount->SetColorAndOpacity(FLinearColor::Red);
	}
}

