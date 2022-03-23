#include "Inventory.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UInventory::Init(int size)
{
	m_nInvenMaxSize = size;
	
	m_AryTotalItems.Reserve(m_nInvenMaxSize);
}

bool UInventory::IsCountAvailable()
{
	return m_AryTotalItems.Num() < m_nInvenMaxSize;
}

int UInventory::GetInvenSize()
{
	return m_nInvenMaxSize;
}

int UInventory::GetAvailalbeStackCount(FName id)
{
	bool HasItem = m_MapMiscItems.Contains(id);

	if (!HasItem)
	{
		return FGlobalVariable::INVEN_MAXSTACK;
	}

	return FGlobalVariable::INVEN_MAXSTACK - m_MapMiscItems[id];
}

bool UInventory::AddItem(FName id, int amount)
{
	bool HasItem = m_MapMiscItems.Contains(id);

	if (!HasItem)
	{
		if(!IsCountAvailable())
		{
			return false;	
		}
		amount = FMath::Min(amount,FGlobalVariable::INVEN_MAXSTACK);
		
		m_AryTotalItems.Add(id);
		
		m_MapMiscItems.Add(id,amount);
	}
	else
	{
		int& CrntAmount = m_MapMiscItems[id];
		
		CrntAmount += amount;
		
		CrntAmount = FMath::Min(CrntAmount,FGlobalVariable::INVEN_MAXSTACK);
	}
	
	m_OnInvenChanged.Broadcast();

	m_OnNewItemAdded.Broadcast(id);

	m_OnItemObtain.Broadcast(UMyLib::GetItemData(id), amount);

	return true;
}

bool UInventory::AddEquipItem(FName gid, int lv)
{
	if(!IsCountAvailable())
	{
		return false;
	}

	m_AryTotalItems.Add(gid);
		
	m_MapEquipItems.Add(gid,lv);

	m_OnInvenChanged.Broadcast();

	return true;
}

void UInventory::RemoveItem(FName id, int amount)
{
	int& Amount = m_MapMiscItems[id];

	if (amount == -1 || Amount <= amount)
	{
		m_MapMiscItems.Remove(id);
		
		m_AryTotalItems.Remove(id);
		
	}
	else
	{
		Amount -= amount;
	}
	m_OnNewItemAdded.Broadcast(id);
	m_OnInvenChanged.Broadcast();
}

void UInventory::RemoveEquipItem(FName gid)
{
	m_MapEquipItems.Remove(gid);
	
	m_AryTotalItems.Remove(gid);

	m_OnInvenChanged.Broadcast();
}

bool UInventory::IsEquipItem(FName hasID)
{
	return !m_MapMiscItems.Contains(hasID);
}

int UInventory::GetItemStack(FName ID)
{
	return m_MapMiscItems[ID];
}

int UInventory::GetItemLevel(FName gID)
{
	return m_MapEquipItems[gID];
}

void UInventory::AddItemLevel(FName gID, int addlv)
{
	m_MapEquipItems[gID]+=addlv;	
}

void UInventory::SubItemLevel(FName gID, int sublv)
{
	m_MapEquipItems[gID]-=sublv;
}

const TArray<FName>& UInventory::GetAryTotalItemIDs() const
{
	return m_AryTotalItems;
}

FName UInventory::GetItemID(int index)
{
	if (index < 0 || m_AryTotalItems.Num() <= index)
	{
		return NAME_None;
	}
	
	return m_AryTotalItems[index];
}

bool UInventory::HasItem(const FName& name, int amount)
{
	if(!m_MapMiscItems.Contains(name))
	{
		return false;
	}

	return m_MapMiscItems[name] >= amount;
}

