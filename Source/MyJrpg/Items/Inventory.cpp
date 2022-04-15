#include "Inventory.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UInventory::Init(int size)
{
	m_nInvenMaxSize = size;
	
	m_AryTotalItems.Reserve(m_nInvenMaxSize);
}

bool UInventory::IsCountAvailable(int addMore)
{
	return m_AryTotalItems.Num() + addMore< m_nInvenMaxSize;
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

int UInventory::GetRemainSlotCount()
{
	return m_nInvenMaxSize - m_AryTotalItems.Num();
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
	FName Oid = UMyLib::GetEquipIDFromHashID(gid);

	m_AryTotalItems.Add(gid);

	if(m_MapEquipItemIdGroup.Contains(Oid))
	{
		m_MapEquipItemIdGroup[Oid].Add(gid,lv);
	}
	else
	{
		m_MapEquipItemIdGroup.Emplace(Oid,TEquipLevelPair()).Add(gid,lv);
	}

	m_OnInvenChanged.Broadcast();

	m_OnItemObtain.Broadcast(UMyLib::GetItemData(gid), 1);

	return true;
}

void UInventory::RemoveItem(FName id, int amount)
{
	int& Amount = m_MapMiscItems[id];

	if (amount == -1 || Amount <= amount)
	{
		m_MapMiscItems.Remove(id);
		
		m_AryTotalItems.Remove(id);

		m_OnNewItemRemoved.Broadcast(id);
	}
	else
	{
		Amount -= amount;
	}
	
	m_OnInvenChanged.Broadcast();
}

void UInventory::RemoveEquipItem(FName gid)
{
	FName Oid = UMyLib::GetEquipIDFromHashID(gid);

	TMap<FName, int>& EquipIDMap = m_MapEquipItemIdGroup[Oid];

	EquipIDMap.Remove(gid);

	if (EquipIDMap.Num() <= 0)
	{
		m_MapEquipItemIdGroup.Remove(Oid);

		m_OnNewItemRemoved.Broadcast(Oid);
	}
	
	m_AryTotalItems.Remove(gid);

	m_OnInvenChanged.Broadcast();
}

bool UInventory::IsEquipItem(FName hasID)
{
	return !m_MapMiscItems.Contains(hasID);
}

int UInventory::GetItemStack(FName ID)
{
	if(!m_MapMiscItems.Contains(ID))
	{
		return 0;
	}
	return m_MapMiscItems[ID];
}

int UInventory::GetItemLevel(FName gID)
{
	FName Oid = UMyLib::GetEquipIDFromHashID(gID);

	return m_MapEquipItemIdGroup[Oid][gID];
}

void UInventory::AddItemLevel(FName gID, int addlv)
{
	FName Oid = UMyLib::GetEquipIDFromHashID(gID);
	
	m_MapEquipItemIdGroup[Oid][gID]+=addlv;	
}

void UInventory::SubItemLevel(FName gID, int sublv)
{
	FName Oid = UMyLib::GetEquipIDFromHashID(gID);
	
	m_MapEquipItemIdGroup[Oid][gID]-=sublv;
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
	
	return  m_AryTotalItems[index];
}

bool UInventory::FindMisItem(const FName& name, int amount)
{
	if(!m_MapMiscItems.Contains(name))
	{
		return false;
	}

	return m_MapMiscItems[name] >= amount;
}

int UInventory::GetUsingSlotCount()
{
	return m_AryTotalItems.Num();
}

const FName* UInventory::FindEquipItem(const FName& Oid, int lv)
{
	FName Id = UMyLib::GetEquipIDFromHashID(Oid);
	
	if (!m_MapEquipItemIdGroup.Contains(Id))
	{
		return nullptr;
	}

	return m_MapEquipItemIdGroup[Id].FindKey(lv);
}

const FName* UInventory::FindEquipItem(const FName& Oid)
{
	FName Id = UMyLib::GetEquipIDFromHashID(Oid);
	
	if (!m_MapEquipItemIdGroup.Contains(Id))
	{
		return nullptr;
	}

	for(const TTuple<FName, int>& Pair :  m_MapEquipItemIdGroup[Id])
	{
		if(Pair.Value >= 0)
		{
			return &Pair.Key;
		}
	}
	return nullptr;
}

int UInventory::GetEquipItemCount(const FName& Oid, int lv)
{
	int Sum = 0;
	
	FName Id = UMyLib::GetEquipIDFromHashID(Oid);
	
	if (!m_MapEquipItemIdGroup.Contains(Id))
	{
		return Sum;
	}

	for(const TTuple<FName, int>& Pair :  m_MapEquipItemIdGroup[Id])
	{
		if(Pair.Value == lv)
		{
			Sum++;
		}
	}
	return Sum;
}
