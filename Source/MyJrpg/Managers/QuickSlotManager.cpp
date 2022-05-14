#include "QuickSlotManager.h"

bool UQuickSlotManager::EquipQuickSlot(int cnt)
{
	if(!TryUnequipQuickSlot())
	{
		return false;
	}

	m_BeltSlots	= NewObject<UInventory>(this);

	m_BeltSlots->Init(cnt);

	m_OnBeltChanged.Broadcast();
	
	return true;
}

bool UQuickSlotManager::TryUnequipQuickSlot()
{
	if(m_BeltSlots->GetUsingSlotCount() > 0)
	{
		return false;
	}

	m_BeltSlots = nullptr;

	m_OnBeltChanged.Broadcast();
	
	return true;
}

bool UQuickSlotManager::EquipBag(int cnt)
{
	if(!TryUnequipBag())
	{
		return false;
	}

	m_BagInven	= NewObject<UInventory>(this);

	m_BagInven->Init(cnt);

	m_OnBeltChanged.Broadcast();
	
	return true;
}

bool UQuickSlotManager::TryUnequipBag()
{
	if(m_BagInven->GetUsingSlotCount() > 0)
	{
		return false;
	}

	m_BagInven = nullptr;

	m_OnBagChanged.Broadcast();
	
	return true;
}

const UInventory* UQuickSlotManager::GetInven() const
{
	return m_BagInven;
}

const UInventory* UQuickSlotManager::GetSlot() const
{
	return m_BeltSlots;
}
