 // All right Reserve 2021 HereticByte


#include "ShopManager.h"

#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"

void UShopManager::BuyItem(const FItemTradingData& item_trading_data, int amount)
{
	int TotalCost = item_trading_data.GetCost()*amount;
	
	if(!UMyGameInstance::Get->m_CurrencyManager->CheckGoldEnough(TotalCost))
	{
		return;
	}
	
	if (!UMyLib::GetPlayerInven()->AddItem(item_trading_data.m_ItemDataRowHandle.RowName,amount))
	{
		return;
	}
	
	UMyGameInstance::Get->m_CurrencyManager->SubGold(TotalCost);
	
	m_OnItemBought.Broadcast(item_trading_data.m_ItemDataRowHandle.GetRow<FItemDataRow>("")->m_TextShowingName);
}

void UShopManager::SellItem(const FName& ability_spec, int amount)
{
	int SellGold = amount * UMyLib::GetItemData(ability_spec).m_nSellValue;
		
	UMyLib::GetPlayerInven()->RemoveItem(ability_spec,amount);

	UMyGameInstance::Get->m_CurrencyManager->AddGold(SellGold);
}
