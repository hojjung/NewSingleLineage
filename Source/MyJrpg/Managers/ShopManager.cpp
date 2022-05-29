 // All right Reserve 2021 HereticByte


#include "ShopManager.h"

#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"

void UShopManager::BuyItem(const FName& itemID, int amount)
{
	const FItemDataRow* ItemDataRow = UItemData::GetItemTable->FindRow<FItemDataRow>(itemID, "");

	int TotalCost = ItemDataRow->m_nPlayerSpentGoldBuy * amount;
	
	if(!UMyGameInstance::Get->m_CurrencyManager->CheckGoldEnough(TotalCost))
	{
		return;
	}
	
	FItemSpec ItemSpec;
	ItemSpec.m_ID = itemID;
	ItemSpec.m_nLvStack = amount;
	
	if(!UMyLib::GetPlayerInven()->HasSpace(ItemSpec))
	{
		return;	
	}
	UMyLib::GetPlayerInven()->AddItem(ItemSpec,UMyLib::IsEquip(itemID));
	
	UMyGameInstance::Get->m_CurrencyManager->SubGold(TotalCost);
	
	m_OnItemBought.Broadcast(itemID);
}

void UShopManager::SellItem(const FName& itemID, int amount)
{
	const FItemDataRow& ItemDataRow = UMyLib::GetItemData(itemID);
	
	if(!UMyLib::GetPlayerInven()->RemoveItem(itemID,amount))
	{
		return;
	}
	
	int SellGold = amount * ItemDataRow.m_nPlayerEarnGoldSell;
	
	UMyGameInstance::Get->m_CurrencyManager->AddGold(SellGold);

	m_OnItemSell.Broadcast(itemID);
}

 void UShopManager::AddTradeItemData(const FName& traderID, const FName& itemID)
 {
	if(m_MapTraderItems.Contains(traderID))
	{
		m_MapTraderItems[traderID].Emplace(itemID);
		return;
	}
	m_MapTraderItems.Emplace(traderID, TArray<FName>(&itemID,1));
 }

 const TArray<FName>* UShopManager::GetShopItems(const FName& traderID)
 {
	if(m_MapTraderItems.Contains(traderID))
	{
		return &m_MapTraderItems[traderID];	
	}
	return nullptr;
 }
