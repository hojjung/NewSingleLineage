#include "QuickSlotManager.h"

#include "MyJrpg/Items/Item_Exe/ItemExecuteBase.h"

void UQuickSlotManager::RegisterItem(TSubclassOf<UItemExecuteBase> exe)
{
	TStrongObjectPtr<UItemExecuteBase>* ItemFound = m_MapItemExe.Find(exe);
	
	if(ItemFound)
	{
		return;
	}

	TStrongObjectPtr<UItemExecuteBase> NewItem = TStrongObjectPtr<UItemExecuteBase>( NewObject<UItemExecuteBase>(this, exe));
	
	m_MapItemExe.Emplace(exe, NewItem);
}

void UQuickSlotManager::UnregisterItem(TSubclassOf<UItemExecuteBase> exe)
{
	m_MapItemExe.Remove(exe);
}

int UQuickSlotManager::ExeItem(TSubclassOf<UItemExecuteBase> exe,UInventory* inven, int index, int cnt)
{
	auto& ItemInst = m_MapItemExe[exe];
	int Iter = -1;
	while (++Iter < cnt)
	{
		ItemInst->Use(inven->GetItemRef(index),inven);
	}
	inven->RemoveItem(index,cnt);
	return inven->GetItemRef(index).m_nLvStack;
}

int UQuickSlotManager::ExeItem(TSubclassOf<UItemExecuteBase> exe, UInventory* inven, FItemSpec& item, int cnt)
{
	auto& ItemInst = m_MapItemExe[exe];
	int Iter = -1;
	while (++Iter < cnt)
	{
		ItemInst->Use(item, inven);
	}
	inven->RemoveItem(item,cnt);
	return item.m_nLvStack;
}
