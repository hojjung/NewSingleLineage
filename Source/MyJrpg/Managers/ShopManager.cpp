// All right Reserve 2021 HereticByte


#include "ShopManager.h"

#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"

void UShopManager::BuyItem(const FItemTradingData& item_trading_data, int amount)
{
	if(!UMyLib::GetPlayerInven()->CheckEmptySlot(1))
	{
		return;
	}

	int TotalCost = item_trading_data.GetCost()*amount;
	
	if(!UMyGameInstance::Get->m_CurrencyManager->SubGold(TotalCost))
	{
		return;
	}

	int Amount = amount;

	UMyLib::GetPlayerInven()->AddItem(item_trading_data.m_ItemDataRowHandle.RowName,Amount);
	
	m_OnItemBought.Broadcast(item_trading_data.m_ItemDataRowHandle.GetRow<FItemDataRow>("")->m_TextShowingName);
}

void UShopManager::SellItem(const FItemSpec& ability_spec, int amount)
{
	int SellGold = amount * UMyLib::GetItemData(ability_spec).m_nSellValue;
		
	if(ability_spec.m_nStack == amount)
	{
		UMyLib::GetPlayerInven()->RemoveItem(ability_spec);
	}
	else
	{
		UMyLib::GetPlayerInven()->RemoveItem(ability_spec.m_ItemID,amount);
	}

	UMyGameInstance::Get->m_CurrencyManager->AddGold(SellGold);
}
