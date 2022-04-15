#include "ItemExecuteManager.h"
#include "EquipManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "Templates/SubclassOf.h"

void UItemExecuteManager::Init()
{
	UMyLib::GetPlayerInven()->m_OnNewItemAdded.AddUObject(this,&UItemExecuteManager::AddUseItem);

	UMyLib::GetPlayerInven()->m_OnNewItemRemoved.AddUObject(this,&UItemExecuteManager::RemoveItem);
}

void UItemExecuteManager::AddUseItem(const FName& itemKey)
{
	if(m_MapItems.Contains(itemKey))
	{
		return;
	}
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(itemKey);

	if(!ItemData.m_ClassExeItem->IsValidLowLevel())
	{
		return;//장비끼우기도 결국 따로 만들어줘야함
	}

	UItemExecuteBase* ConsumeItem = NewObject<UItemExecuteBase>(this,ItemData.m_ClassExeItem);

	m_MapItems.Add(itemKey,ConsumeItem);

	m_AryItems.Add(ConsumeItem);
}

void UItemExecuteManager::RemoveItem(const FName& itemKey)
{
	if(!m_MapItems.Contains(itemKey))
	{
		return;
	}
	UItemExecuteBase* ItemWantErase = m_MapItems[itemKey];
	
	m_MapItems.Remove(itemKey);

	m_AryItems.Remove(ItemWantErase);
}

void UItemExecuteManager::EquipUnequipItem(const FName& itemSpec)
{
	if(UMyGameInstance::Get->m_EquipManager->IsItemEquipped(itemSpec))
	{
		PRINTF("EquipUnequipItem01");
		UMyGameInstance::Get->m_EquipManager->Unequip(itemSpec);
	}
	else
	{
		PRINTF("EquipUnequipItem02");
		UMyGameInstance::Get->m_EquipManager->Equip(UMyLib::GetItemData(itemSpec).m_ItemType,itemSpec);
	}
}

void UItemExecuteManager::ExecuteItem(const FName& itemID,UInventory* inven)//애초에 이건 장비템에 대해 전혀 신경안썼음,퀵슬롯에 장비템이 존재하는 건?
{
	m_MapItems[itemID]->Use(itemID,inven);

	if(m_OnItemUse.IsBound())
	{
		m_OnItemUse.Broadcast(itemID);
	}
}