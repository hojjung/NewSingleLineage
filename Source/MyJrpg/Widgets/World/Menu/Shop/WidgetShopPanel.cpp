#include "WidgetShopPanel.h"

#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"

void UWidgetShopPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bNoFilter = true;

	m_InvenPanel->Init(UMyLib::GetPlayerInven(),EPanelType::ShopSell);
	
	m_FilterBtns->RegisterFilter(m_InvenPanel);
	m_FilterBtns->RegisterFilter(this);
}

FReply UWidgetShopPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetShopPanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void UWidgetShopPanel::UnFocusCurrent()
{
	if(m_CurrentFocused.Get())
	{
		m_CurrentFocused.Get()->SetMyUnFocus();

		m_CurrentFocused= nullptr;
	}
}

void UWidgetShopPanel::OnFocus(UUWidgetShopItemElement* ele)
{
	UnFocusCurrent();
	
	m_CurrentFocused = ele;
}

bool UWidgetShopPanel::IsSameType(const FItemTradingData* item)
{
	if(m_bNoFilter)
	{
		return true;
	}
	
	EItemType Type = UMyLib::GetItemType(item->m_ItemDataRowHandle.RowName);
	
	return Type == m_FilterCategoryItem;
}

void UWidgetShopPanel::ClearFilter()
{
	UnFocusCurrent();
	
	m_bNoFilter = true;

	m_FilterCategoryItem = EItemType::None;

	UpdateShopPanel();
}

void UWidgetShopPanel::OnFilterMisc()
{
	SetItemFilter(EItemType::None);
}

void UWidgetShopPanel::OnFilterConsumable()
{
	SetItemFilter(EItemType::Consume);
}

void UWidgetShopPanel::OnFilterEquips()
{
	SetItemFilter(EItemType::Equip);
}

void UWidgetShopPanel::SetShopPanel(const UDataTable* shopTable)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_AryItemEles.Reset();

	m_AryShopData.Reset();
	
	m_InvenBox->ClearChildren();

	shopTable->GetAllRows("",m_AryShopData);

	for(const FItemTradingData* TradeData : m_AryShopData)
	{
		UUWidgetShopItemElement* ItemEle = CreateWidget<UUWidgetShopItemElement>(this, m_ClassWidgetItemEle);

		ItemEle->Clear();

		m_InvenBox->AddChildToWrapBox(ItemEle)->SetPadding(FMargin(2));
		
		ItemEle->m_OnFocus.AddUObject(this,&UWidgetShopPanel::OnFocus);
		
		m_AryItemEles.Add(ItemEle);
	}
	
	ClearFilter();

	UpdateShopPanel();
}

void UWidgetShopPanel::UpdateShopPanel()
{
	int ItemIndex = 0;
	int Index = 0;
	
	for (const FItemTradingData* ShopData : m_AryShopData)
	{
		if(!IsSameType(ShopData))
		{
			ItemIndex++;
			continue;
		}
		
		m_AryItemEles[Index]->UpdateElement(*ShopData);

		Index++;
		ItemIndex++;
	}

	for(; Index<m_AryItemEles.Num();Index++)
	{
		m_AryItemEles[Index]->Clear();
	}
}

void UWidgetShopPanel::SetItemFilter(EItemType typeWant)
{
	UnFocusCurrent();
	
	m_bNoFilter = false;

	m_FilterCategoryItem = typeWant;

	UpdateShopPanel();
}

void UWidgetShopPanel::ClosePanel()
{
	Super::ClosePanel();

	UnFocusCurrent();

	m_AryItemEles.Reset();

	m_AryShopData.Reset();
	
	m_InvenBox->ClearChildren();
}
