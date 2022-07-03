#include "QuickSlotManager.h"

#include "MyGameInstance.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Item_Exe/ItemExecuteBase.h"


void UQuickSlotManager::RegisterQuickItemExe(const FItemDataRow& itemData)
{
	if(!itemData.m_ClassExeItem->IsValidLowLevel())
	{
		return;
	}
	RegisterItem(itemData.m_ClassExeItem);
}

void UQuickSlotManager::UnregisterQuickItemExe(const FItemDataRow& itemData)
{
	if(!itemData.m_ClassExeItem->IsValidLowLevel())
	{
		return;
	}
	UnregisterItem(itemData.m_ClassExeItem);
}

void UQuickSlotManager::RegisterItem(TSubclassOf<UItemExecuteBase> exe)
{
	FQuickCount* ItemFound = m_MapItemExe.Find(exe);
	
	if(ItemFound)
	{
		ItemFound->m_nAmount++;//스택 개수가 아니라 인벤토리 개수일텐데
		return;
	}

	m_MapItemExe.Emplace(exe, FQuickCount(exe, this));
}

void UQuickSlotManager::UnregisterItem(TSubclassOf<UItemExecuteBase> exe)
{
	FQuickCount* ItemFound = m_MapItemExe.Find(exe);
	
	if(!ItemFound)
	{
		return;
	}
	ItemFound->m_nAmount--;

	if(ItemFound->m_nAmount <= 0)
	{
		m_MapItemExe.Remove(exe);
	}
}

int UQuickSlotManager::ExeItem(TSubclassOf<UItemExecuteBase> exe,UInventory* inven, int index, int cnt)
{
	return ExeItem(exe,inven,inven->GetItemRef(index),cnt);
}

int UQuickSlotManager::ExeItem(TSubclassOf<UItemExecuteBase> exe, UInventory* inven, FItemSpec& item, int cnt)
{
	auto& ItemInst = m_MapItemExe[exe];
	int Iter = -1;
	while (++Iter < cnt)
	{
		ItemInst.m_ItemExe->Use(item, inven);
	}
	inven->RemoveItem(item,cnt);
	return item.m_nLvStack;
}