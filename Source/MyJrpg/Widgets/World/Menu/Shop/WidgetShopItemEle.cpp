// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetShopItemEle.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"


void UWidgetShopItemEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_ItemID = NAME_None;

	m_ElementBase->m_OnHold.AddUObject(this,&UWidgetShopItemEle::OnHold);

	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
}

FText UWidgetShopItemEle::GetFocusText()
{
	return NSLOCTEXT("UWidgetShopItemEle","FocusBuy","Buy?");
}

void UWidgetShopItemEle::OnHold(UWidgetBaseElement* ele)
{
	SetMyUnFocus();
	
	UMyLib::GetCanvas()->OpenItemInfoData(UMyLib::GetItemData(m_ItemID));
}

void UWidgetShopItemEle::OpenStackCalculator()
{
	PRINTF("UWidgetShopItemEle::TryBuyItem");

	int Gold = UMyGameInstance::Get->m_CurrencyManager->GetGold();

	const FItemDataRow& ItemData = *UItemData::GetItemTable->FindRow<FItemDataRow>(m_ItemID,"");
	
	int Cost = ItemData.m_nPlayerSpentGoldBuy; 

	if(Cost<=0)
	{
		m_nMaxAmount = 10;
	}
	else
	{
		int AbleToBuy = Gold / Cost;

		int AbleToDeposit = UMyGameInstance::Get->m_Inven->GetAvailableCount(m_ItemID);

		m_nMaxAmount = FMath::Min(AbleToBuy,AbleToDeposit);
	}
	UWidgetStackCalculator* Calculator = UMyLib::GetCanvas()->GetCalculator();

	Calculator->m_OnNumberAccept.AddUObject(this,&UWidgetShopItemEle::OnBuyConfirm);

	Calculator->m_OnGetMax.BindUObject(this,&UWidgetShopItemEle::GetMaxAmount);

	Calculator->Open(0);
}

void UWidgetShopItemEle::OnClicked()
{
	if(IsSlotEmpty())
	{
		return;
	}

	if(m_TextFocus->IsVisible())
	{
		if(UMyLib::IsEquip(m_ItemID))
		{
			UMyGameInstance::Get->m_ShopManager->BuyItem(m_ItemID,1);
		}
		else
		{
			OpenStackCalculator();
		}
		SetMyUnFocus();
		return;
	}

	m_OnFocus.Broadcast(this);//다른애 풀림

	m_TextFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextFocus->SetText(GetFocusText());

	m_ElementBase->SetMyFocus();
}

bool UWidgetShopItemEle::IsSlotEmpty()
{
	return m_ItemID.IsNone();
}

void UWidgetShopItemEle::OnBuyConfirm(int amount)
{
	UMyGameInstance::Get->m_ShopManager->BuyItem(m_ItemID,amount);
}

int UWidgetShopItemEle::GetMaxAmount()
{
	return m_nMaxAmount;
}

void UWidgetShopItemEle::SetMyUnFocus()
{
	m_TextFocus->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ElementBase->SetMyUnFocus();
}

void UWidgetShopItemEle::UpdateElement(const FName& tradeData)
{
	m_ItemID = tradeData;
	
	m_TextCost->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FItemDataRow& ItemData = *UItemData::GetItemTable->FindRow<FItemDataRow>(m_ItemID,"");

	m_TextCost->SetText(FText::AsNumber(ItemData.m_nPlayerSpentGoldBuy));

	m_ElementBase->SetHoldable(true);

	m_ElementBase->SetItemData(ItemData);
}

void UWidgetShopItemEle::Clear()
{
	m_ItemID = NAME_None;
	
	m_ElementBase->Clear();

	m_ElementBase->SetHoldable(false);

	m_TextCost->SetVisibility(ESlateVisibility::Collapsed);
}
