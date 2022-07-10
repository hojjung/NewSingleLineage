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

	m_ItemElement->m_OnHold.AddUObject(this,&UWidgetCraftCostElement::OnHold);

	m_ItemElement->SetHoldable(true);

	m_ItemElement->SetFocusable(false);

	m_ItemElement->SetDragable(false);
	
	UpdateCostAmount();
}

void UWidgetCraftCostElement::BoundStackDefaultStackFunPtr()
{
	m_GetStackFuncPtr.BindUObject(this, &UWidgetCraftCostElement::GetStack);
}

UWidgetBaseElement* UWidgetCraftCostElement::GetEle()
{
	return m_ItemElement;
}

void UWidgetCraftCostElement::UpdateCostAmount()
{
	FString FormatAmount;

	int Stack = m_GetStackFuncPtr.Execute(m_nIndex);

	if(UMyLib::IsEquip(m_CraftData->m_ItemDataRowHandle.RowName))
	{
		if(m_CraftData->m_nStackOrLevel <= 0)
		{
			FormatAmount = FString::Printf(TEXT("%d/1"), Stack);
		}
		else
		{
			FormatAmount = FString::Printf(TEXT("Lv.%d  %d/1"),m_CraftData->m_nStackOrLevel, Stack);	
		}

		if(Stack > 0)
		{
			m_TextCostAmount->SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			m_TextCostAmount->SetColorAndOpacity(FLinearColor::Red);
		}
	}
	else
	{
		FormatAmount = FString::Printf(TEXT("%d/%d"),Stack,m_CraftData->m_nStackOrLevel);

		if(Stack >= m_CraftData->m_nStackOrLevel)
		{
			m_TextCostAmount->SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			m_TextCostAmount->SetColorAndOpacity(FLinearColor::Red);
		}
	}
	
	FText TextWant = FText::FromString(FormatAmount); 
	
	m_TextCostAmount->SetText(TextWant);
}

void UWidgetCraftCostElement::OnHold(UWidgetBaseElement* ele)
{
	ele->SetMyUnFocus();
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(m_CraftData->m_ItemDataRowHandle.RowName);
	
	UMyLib::OpenItemInfo(ItemData);
}

int UWidgetCraftCostElement::GetStack(int index)
{
	return UMyLib::GetItemCountAllInven(m_CraftData->m_ItemDataRowHandle.RowName,m_CraftData->m_nStackOrLevel);
}