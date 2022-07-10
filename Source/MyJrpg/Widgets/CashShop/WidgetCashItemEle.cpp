#include "WidgetCashItemEle.h"

void UWidgetCashItemEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnPurchase->OnClicked.AddDynamic(this, &UWidgetCashItemEle::OnPurchaseTry);
}

void UWidgetCashItemEle::SetCashItem(const FCashProductDataRow& cashData)
{
	m_CashData = &cashData;

	m_TextName->SetText(m_CashData->m_ProductName);

	m_TextDesc->SetText(m_CashData->m_ProductDesc);

	m_ImgIcon->SetBrushFromSoftTexture(m_CashData->m_Icon);
	
	if(m_CashData->m_fSale > 1.f)
	{
		UpdateSale();
	}
	else
	{
		m_OverlaySale->SetVisibility(ESlateVisibility::Collapsed);
		
		m_TextPriceBefore->SetVisibility(ESlateVisibility::Collapsed);
		
		m_OverlayBeforePrice->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidgetCashItemEle::OnPurchaseTry()
{
		
}

void UWidgetCashItemEle::UpdateSale()
{
	m_TextPriceBefore->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_OverlaySale->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_OverlayBeforePrice->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	int KRW = m_CashData->m_KRW;
	
	float Sale = m_CashData->m_fSale / 100.f;//0.5f =>>> 50
	
	int BeforeKRW = KRW / Sale; 
	
	FString Price = FString::Printf(TEXT("%d KRW"),KRW);
	
	m_TextPrice->SetText(FText::FromString(Price));
	
	FString PriceBefore = FString::Printf(TEXT("%d KRW"),BeforeKRW);

	m_TextPriceBefore->SetText(FText::FromString(PriceBefore));
	//
	FString SaleF = FString::Printf(TEXT("-%d%%"), (int)m_CashData->m_fSale);
	
	m_TextDiscount->SetText(FText::FromString(SaleF));
}



