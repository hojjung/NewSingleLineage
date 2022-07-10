#include "WidgetCashShopCanvas.h"

#include "Components/WrapBox.h"

void UWidgetCashShopCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateAllElements();
	
	OnOpenPackage();

	m_BtnPackage->OnClicked.AddDynamic(this, &UWidgetCashShopCanvas::OnOpenPackage);
	m_BtnBuild->OnClicked.AddDynamic(this, &UWidgetCashShopCanvas::OnOpenBuild);
	m_BtnEquip->OnClicked.AddDynamic(this, &UWidgetCashShopCanvas::OnOpenEquip);
	m_BtnMisc->OnClicked.AddDynamic(this, &UWidgetCashShopCanvas::OnOpenMisc);
	m_BtnCurrency->OnClicked.AddDynamic(this, &UWidgetCashShopCanvas::OnOpenCurrency);
}

void UWidgetCashShopCanvas::CreateAllElements()
{
	TArray<const FCashProductDataRow*> AryRows;
	
	UCashProductTable::GetCashTable->GetAllRows("", AryRows);

	for(const FCashProductDataRow* Data : AryRows)
	{
		CreateCashWidgetsToCategory(*Data);
	}
}

void UWidgetCashShopCanvas::CloseAllPanel()
{
	m_ScrollPackage->SetVisibility(ESlateVisibility::Collapsed);

	m_ScrollBuild->SetVisibility(ESlateVisibility::Collapsed);

	m_ScrollEquipment->SetVisibility(ESlateVisibility::Collapsed);

	m_ScrollMisc->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ScrollCurrency->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetCashShopCanvas::CreateCashWidgetsToCategory(const FCashProductDataRow& data)
{
	UWidgetCashItemEle* Ele = CreateWidget<UWidgetCashItemEle>(this, m_ClassCashItem);

	Ele->SetCashItem(data);
	
	switch (data.m_Category)
	{
	case ECashCategory::Package:
		Cast<UWrapBox>(m_ScrollPackage->GetChildAt(0))->AddChildToWrapBox(Ele);
		break;
	case ECashCategory::Build:
		Cast<UWrapBox>(m_ScrollBuild->GetChildAt(0))->AddChildToWrapBox(Ele);
		break;
	case ECashCategory::Equipment:
		Cast<UWrapBox>(m_ScrollEquipment->GetChildAt(0))->AddChildToWrapBox(Ele);
		break;
	case ECashCategory::Misc:
		Cast<UWrapBox>(m_ScrollMisc->GetChildAt(0))->AddChildToWrapBox(Ele);
		break;
	case ECashCategory::Currency:
		Cast<UWrapBox>(m_ScrollCurrency->GetChildAt(0))->AddChildToWrapBox(Ele);
		break;
	}
}

void UWidgetCashShopCanvas::OnOpenPackage()
{
	CloseAllPanel();
	m_ScrollPackage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCashShopCanvas::OnOpenBuild()
{
	CloseAllPanel();
	m_ScrollBuild->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCashShopCanvas::OnOpenEquip()
{
	CloseAllPanel();
	m_ScrollEquipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCashShopCanvas::OnOpenMisc()
{
	CloseAllPanel();
	m_ScrollMisc->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCashShopCanvas::OnOpenCurrency()
{
	CloseAllPanel();
	m_ScrollCurrency->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
