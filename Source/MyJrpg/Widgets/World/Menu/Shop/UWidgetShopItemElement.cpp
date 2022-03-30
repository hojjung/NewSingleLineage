// Fill out your copyright notice in the Description page of Project Settings.


#include "UWidgetShopItemElement.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UUWidgetShopItemElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_TradeData = nullptr;

	m_ElementBase->m_OnHold.AddUObject(this,&UUWidgetShopItemElement::OnHoldingComplete);

	m_ElementBase->m_OnClick.AddUObject(this,&UUWidgetShopItemElement::OnClicked);

	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
}

FText UUWidgetShopItemElement::GetFocusText()
{
	return NSLOCTEXT("UUWidgetShopItemElement","FocusBuy","Buy?");
}

void UUWidgetShopItemElement::OnHoldingComplete()
{
	FName ID = m_TradeData->m_ItemDataRowHandle.RowName;
	
	UMyLib::GetCanvas()->OpenItemInfo(EItemInfo::Shop,ID,nullptr);

	SetMyUnFocus();
}

void UUWidgetShopItemElement::OpenStackCalculator()
{
	PRINTF("UUWidgetShopItemElement::TryBuyItem");

	int Gold = UMyGameInstance::Get->m_CurrencyManager->GetGold();
	
	int Cost = m_TradeData->GetCost(); 

	if(Cost<=0)
	{
		m_nMaxAmount = 10;
	}
	else
	{
		int AbleToBuy = Gold / Cost;

		int AbleToDeposit;

		if(!UMyLib::IsEquip(m_TradeData->m_ItemDataRowHandle.RowName))
		{
			AbleToDeposit = UMyGameInstance::Get->m_Inven->GetItemStack(m_TradeData->m_ItemDataRowHandle.RowName);
		}
		else
		{
			AbleToDeposit = UMyGameInstance::Get->m_Inven->GetRemainSlotCount();
		}

		m_nMaxAmount = FMath::Min(AbleToBuy,AbleToDeposit);
	}

	UWidgetStackCalculator* Calculator = UMyLib::GetCanvas()->OpenCalculator();

	Calculator->m_OnNumberAccept.AddUObject(this,&UUWidgetShopItemElement::OnBuyConfirm);

	Calculator->m_OnGetMax.BindUObject(this,&UUWidgetShopItemElement::GetMaxAmount);
}

void UUWidgetShopItemElement::OnClicked()
{
	if(IsSlotEmpty())
	{
		return;
	}

	if(m_TextFocus->IsVisible())
	{
		OpenStackCalculator();
		SetMyUnFocus();
		return;
	}

	m_OnFocus.Broadcast(this);//다른애 풀림

	m_TextFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextFocus->SetText(GetFocusText());

	m_ElementBase->SetMyFocus();
}

bool UUWidgetShopItemElement::IsSlotEmpty()
{
	return !m_TradeData;
}

void UUWidgetShopItemElement::OnBuyConfirm(int amount)
{
	if(UMyLib::GetPlayerInven()->GetRemainSlotCount() < amount)
	{
		PRINTF("void UUWidgetShopItemElement::OnBuyConfirm(int amount) :: InvenMax");
		return;
	}
	UMyGameInstance::Get->m_ShopManager->BuyItem(*m_TradeData,amount);
}

int UUWidgetShopItemElement::GetMaxAmount()
{
	return m_nMaxAmount;
}

void UUWidgetShopItemElement::SetMyUnFocus()
{
	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ElementBase->SetMyUnFocus();
}

void UUWidgetShopItemElement::UpdateElement(const FItemTradingData& tradeData)
{
	m_TradeData = &tradeData;
	
	m_TextCost->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FItemDataRow& ItemData = *tradeData.m_ItemDataRowHandle.GetRow<FItemDataRow>("");

	m_TextCost->SetText(FText::AsNumber(tradeData.GetCost()));

	m_ElementBase->SetHoldable(true);

	m_ElementBase->SetIcon(ItemData.m_ItemIcon);

	m_ElementBase->SetGlowColor(ItemData.m_ColorHandle);
}

void UUWidgetShopItemElement::Clear()
{
	m_ElementBase->Clear();

	m_ElementBase->SetHoldable(false);

	m_TextCost->SetVisibility(ESlateVisibility::Collapsed);
}
