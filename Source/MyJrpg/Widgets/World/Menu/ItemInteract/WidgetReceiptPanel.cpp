#include "WidgetReceiptPanel.h"

void UWidgetReceiptPanel::SetReceipt(const FItemConvertRow& item_convert_row)
{
	m_Scroll->ClearChildren();
	
	for(const FItemConvertSet& SetWant : item_convert_row.m_AryItems)
	{
		UWidgetReceiptEle* ItemEle = CreateWidget<UWidgetReceiptEle>(this, m_ClassReceiptEle);

		ItemEle->SetElement(SetWant);

		m_Scroll->AddChild(ItemEle);
	}
}
