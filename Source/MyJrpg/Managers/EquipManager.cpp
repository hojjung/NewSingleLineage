#include "EquipManager.h"

#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"

void UEquipManager::Init()
{
	m_AryEqupSlots.Init(nullptr,(int)EEquipSlotType::Length);

	m_Inven = UMyGameInstance::Get->m_Inven;
}

void UEquipManager::Equip(EEquipSlotType slotWant, int invenIndex)
{
	Unequip(slotWant);
	
	FItemSpec& Item = m_Inven->GetItemRef(invenIndex);
	
	int SlotIndex = (int)slotWant;
	
	m_AryEqupSlots[SlotIndex] = &Item;

	m_AryEqupSlots[SlotIndex]->m_bIsEquipped = true;

	//EquipOption(m_AryEqupSlots[Index]);
	
	SetIsRangeStance();
	
	m_OnEquipChanged.Broadcast();
}

void UEquipManager::EquipOption(const FName& itemWant)
{
	UMyGameInstance::Get->m_PlayerStatManager->EquipItem(itemWant);
}

void UEquipManager::UnequipOption(const FName& itemWant)
{
	UMyGameInstance::Get->m_PlayerStatManager->UnequipItem(itemWant);
}

void UEquipManager::Unequip(EEquipSlotType slotWant)
{
	PRINTF("Unequip01");
	int SlotIndex = (int)slotWant;

	FItemSpec* Temp = m_AryEqupSlots[SlotIndex];
	
	if(!Temp)
	{
		return;
	}
	Temp->m_bIsEquipped = false;
	
	//UnequipOption(Temp);
	
	m_AryEqupSlots[SlotIndex] = nullptr;
	
	SetIsRangeStance();
	
	m_OnEquipChanged.Broadcast();
}

void UEquipManager::Unequip(const FName& itemWant)
{
	// int Index = m_AryEqupSlots.Find(itemWant) + 1;
	// EEquipSlotType SlotT = (EEquipSlotType)Index;
	// Unequip(SlotT);
}

bool UEquipManager::IsItemEquipped(EEquipSlotType t, const FItemSpec& itemWant)
{
	int Index = (int) t;

	return m_AryEqupSlots[Index] == &itemWant;
}

bool UEquipManager::IsItemEquipped(const FItemSpec& itemWant)
{
	EEquipSlotType SlotT = UMyLib::GetEquipItemSlot(itemWant.m_ID);
	
	int Index = (int) SlotT;

	return m_AryEqupSlots[Index] == &itemWant;
}

bool UEquipManager::IsItemEquipped(EEquipSlotType wantSlot)
{
	return GetEquipItem((int)wantSlot) != nullptr;
}

const FItemSpec* UEquipManager::GetEquipItem(int slotIndex)
{
	return m_AryEqupSlots[slotIndex];
}

bool UEquipManager::IsRangeStance()
{
	return m_bIsRange;
}

void UEquipManager::SetIsRangeStance()
{
	const auto* FoundItem = GetEquipItem((int)EEquipSlotType::Weapon);

	if(!FoundItem)
	{
		m_bIsRange = false;
		return ;
	}
	
	m_bIsRange = UMyLib::GetItemData(FoundItem->m_ID).m_bIsRange;
}

UParticleSystem* UEquipManager::GetBulletEffect()
{
	const auto* FoundItem = GetEquipItem((int)EEquipSlotType::Weapon);
	
	if(!FoundItem)
	{
		m_bIsRange = false;
		return nullptr;
	}

	const FColorDataRow* ColorData = UMyLib::GetItemData(FoundItem->m_ID).m_ColorHandle.GetRow<FColorDataRow>("");

	return ColorData->m_Bullet;
}

void UEquipManager::UpdateEquip()
{
	m_OnEquipChanged.Broadcast();
}
