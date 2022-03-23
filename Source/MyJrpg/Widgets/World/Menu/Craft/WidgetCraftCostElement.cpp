#include "WidgetCraftCostElement.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftCostElement::SetCraftCost(const FCraftItemCost& cost)
{
	m_CraftData = &cost;
	
	const FItemDataRow* CraftData = m_CraftData->m_ItemDataRowHandle.GetRow<FItemDataRow>("");

	m_ItemElement->SetIcon(CraftData->m_ItemIcon);

	m_ItemElement->SetGlowColor(CraftData->m_ColorHandle);

	m_ItemElement->m_OnHold.AddUObject(this,&UWidgetCraftCostElement::OnHoldComplete);

	m_ItemElement->SetHoldable(true);

	m_ItemElement->SetFocusable(false);

	UpdateCostAmount();
}

void UWidgetCraftCostElement::OnHoldComplete()
{
	PRINTF("UWidgetCraftCostElement::OnHoldComplete");
}

void UWidgetCraftCostElement::UpdateCostAmount()
{
	// FName* ItemSpecFromInven = UMyLib::GetPlayerInven()->GetItem(m_CraftData->m_ItemDataRowHandle.RowName);
	//
	// FName* ItemSpecFromStorage = UMyLib::GetPlayerStorage()->GetItem(m_CraftData->m_ItemDataRowHandle.RowName);
	//
	// int InvenStack = ItemSpecFromInven ? ItemSpecFromInven->m_nStack : 0;
	//
	// int StorageStack = ItemSpecFromStorage ? ItemSpecFromStorage->m_nStack : 0;
	//
	// int AmountCurrent = InvenStack+StorageStack;//창고와 인벤에서 가져와야함
	//
	// int AmountCost = m_CraftData->m_nStackCount * UMyGameInstance::Get->m_CraftManager->GetAmount();
	//
	// FString FormatAmount = FString::Printf(TEXT("%d/%d"),AmountCurrent,AmountCost);
	//
	// FText TextWant = FText::FromString(FormatAmount); 
	//
	// m_TextCostAmount->SetText(TextWant);
	//
	// if(AmountCurrent < AmountCost)
	// {
	// 	m_TextCostAmount->SetColorAndOpacity(FLinearColor::Red);
	// }
	// else
	// {
	// 	m_TextCostAmount->SetColorAndOpacity(FLinearColor::White);
	// }
}

