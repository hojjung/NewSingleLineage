#include "WidgetFilterBtns.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Widgets/World/Menu/Craft/WidgetCraftPanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "MyJrpg/Widgets/World/Menu/Shop/WidgetShopPanel.h"

void UWidgetFilterBtns::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_ItemType = EItemType::None;
	m_BtnClearFilter->IsFocusable = false;
	m_BtnFilterMisc->IsFocusable = false;
	m_BtnFilterConsume->IsFocusable = false;
	m_BtnFilterEquip->IsFocusable = false;
	//
	UMyLib::SetBtnTint(m_BtnClearFilter,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnFilterMisc,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnFilterConsume,FLinearColor::Gray);
	UMyLib::SetBtnTint(m_BtnFilterEquip,FLinearColor::Gray);
	//
	m_BtnClearFilter->OnClicked.AddDynamic(this, &UWidgetFilterBtns::OnFilterClear);
	m_BtnFilterMisc->OnClicked.AddDynamic(this, &UWidgetFilterBtns::OnFilterMisc);
	m_BtnFilterConsume->OnClicked.AddDynamic(this, &UWidgetFilterBtns::OnFilterConsume);
	m_BtnFilterEquip->OnClicked.AddDynamic(this, &UWidgetFilterBtns::OnFilterEquip);
}

void UWidgetFilterBtns::OnBtnPressed(UButton* btn, EItemType t)
{
	if (m_BtnSelected == btn)
	{
		ClearFilter();
		return;
	}
	if(m_BtnSelected)
	{
		UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::Gray);
	}
	m_BtnSelected = btn;

	UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::White);
	
	m_ItemType = t;
}

void UWidgetFilterBtns::RegisterFilter(UWidgetInventory* inven)
{
	m_BtnClearFilter->OnClicked.AddDynamic(inven, &UWidgetInventory::ClearFilter);
	m_BtnFilterMisc->OnClicked.AddDynamic(inven, &UWidgetInventory::OnFilterMisc);
	m_BtnFilterConsume->OnClicked.AddDynamic(inven, &UWidgetInventory::OnFilterConsumable);
	m_BtnFilterEquip->OnClicked.AddDynamic(inven, &UWidgetInventory::OnFilterEquips);
}

void UWidgetFilterBtns::RegisterFilter(UWidgetShopPanel* shop)
{
	m_BtnClearFilter->OnClicked.AddDynamic(shop, &UWidgetShopPanel::ClearFilter);
	m_BtnFilterMisc->OnClicked.AddDynamic(shop, &UWidgetShopPanel::OnFilterMisc);
	m_BtnFilterConsume->OnClicked.AddDynamic(shop, &UWidgetShopPanel::OnFilterConsumable);
	m_BtnFilterEquip->OnClicked.AddDynamic(shop, &UWidgetShopPanel::OnFilterEquips);
}

void UWidgetFilterBtns::RegisterFilter(UWidgetCraftPanel* craft)
{
	m_BtnClearFilter->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::ClearFilter);
	m_BtnFilterMisc->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterItem);
	m_BtnFilterConsume->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterItem);
	m_BtnFilterEquip->OnClicked.AddDynamic(craft, &UWidgetCraftPanel::OnFilterItem);
}

EItemType UWidgetFilterBtns::GetCrntItemType()
{
	return m_ItemType;
}

void UWidgetFilterBtns::ClearFilter()
{
	m_ItemType = EItemType::None;

	if(m_BtnSelected)
		UMyLib::SetBtnTint(m_BtnSelected,FLinearColor::Gray);
	
	m_BtnSelected = nullptr;
}

void UWidgetFilterBtns::OnFilterClear()
{
	OnBtnPressed(m_BtnClearFilter, EItemType::None);
}

void UWidgetFilterBtns::OnFilterMisc()
{
	OnBtnPressed(m_BtnFilterMisc, EItemType::misc);
}

void UWidgetFilterBtns::OnFilterConsume()
{
	OnBtnPressed(m_BtnFilterConsume, EItemType::Consume);
}

void UWidgetFilterBtns::OnFilterEquip()
{
	OnBtnPressed(m_BtnFilterEquip, EItemType::Equip);
}
