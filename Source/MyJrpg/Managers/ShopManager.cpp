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

	if (UMyLib::IsEquip(itemID))
	{
		int Iter = -1;

		while (++Iter < amount)
		{
			FName HashID = UMyLib::GenerateEquipItemHashKey(itemID, this);
			
			if(!UMyLib::GetPlayerInven()->AddEquipItem(HashID))
			{
				break;				
			}
		}
	}
	else
	{
		if(!UMyLib::GetPlayerInven()->AddItem(itemID,amount))
			return;
	}
	
	UMyGameInstance::Get->m_CurrencyManager->SubGold(TotalCost);
	
	m_OnItemBought.Broadcast(itemID);
}

void UShopManager::SellItem(const FName& itemID, int amount)
{
	const FItemDataRow& ItemDataRow = UMyLib::GetItemData(itemID);
	
	int SellGold = amount * ItemDataRow.m_nPlayerEarnGoldSell;

	if (UMyLib::GetItemType(itemID) == EItemType::Equip)
	{
		UMyLib::GetPlayerInven()->RemoveEquipItem(itemID);
	}
	else
	{
		UMyLib::GetPlayerInven()->RemoveItem(itemID,amount);
	}
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
