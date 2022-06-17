#include "WidgetReceiptPanel.h"

#include "Components/ScrollBoxSlot.h"

void UWidgetReceiptPanel::SetReceipt(const FItemConvertRow& item_convert_row)
{
	m_Scroll->ClearChildren();
	
	for(const FItemConvertSet& SetWant : item_convert_row.m_AryItems)
	{
		UWidgetReceiptEle* ItemEle = CreateWidget<UWidgetReceiptEle>(this, m_ClassReceiptEle);

		ItemEle->SetElement(SetWant);

		UPanelSlot* PanelSlotWant = m_Scroll->AddChild(ItemEle);

		Cast<UScrollBoxSlot>(PanelSlotWant)->SetPadding(FMargin(0,40,0,0));
	}
}
