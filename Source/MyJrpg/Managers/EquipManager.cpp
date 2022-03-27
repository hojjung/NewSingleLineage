#include "EquipManager.h"

#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"

void UEquipManager::Init()
{
	m_AryEqupSlots.Init(NAME_None,(int)EEquipSlotType::Length-1);
}

bool UEquipManager::Equip(EEquipSlotType slotWant,const FName& itemWant)
{
	int Index = (int)slotWant - 1;
	
	m_AryEqupSlots[Index] = itemWant;
	
	EquipOption(m_AryEqupSlots[Index]);
	
	SetIsRangeStance();
	
	m_OnEquipChanged.Broadcast();

	return true;
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
	int Index = (int)slotWant - 1;
	
	const FName& Temp = m_AryEqupSlots[Index];

	UnequipOption(Temp);

	m_AryEqupSlots[Index] = NAME_None;

	SetIsRangeStance();
	m_OnEquipChanged.Broadcast();
}

void UEquipManager::Unequip(const FName& itemWant)
{
	int Index = m_AryEqupSlots.Find(itemWant) + 1;
	EEquipSlotType SlotT = (EEquipSlotType)Index;
	Unequip(SlotT);
}

const TArray<FName>& UEquipManager::GetEquipAry() const
{
	return m_AryEqupSlots;
}

bool UEquipManager::IsItemEquipped(const FName& itemWant)
{
	EEquipSlotType SlotT = UMyLib::GetEquipItemSlot(itemWant);
	
	int Index = (int) SlotT - 1;

	return m_AryEqupSlots[Index] == itemWant;
}

bool UEquipManager::IsItemEquipped(EEquipSlotType wantSlot)
{
	return !GetEquipItem(wantSlot).IsNone();
}

const FName& UEquipManager::GetEquipItem(EEquipSlotType wantSlot)
{
	return m_AryEqupSlots[(int)wantSlot-1];
}

bool UEquipManager::IsRangeStance()
{
	return m_bIsRange;
}

void UEquipManager::SetIsRangeStance()
{
	const FName& FoundItem = GetEquipItem(EEquipSlotType::Weapon);

	if(FoundItem.IsNone())
	{
		m_bIsRange = false;
		return ;
	}
	
	m_bIsRange = UMyLib::GetItemData(FoundItem).m_bIsRange;
}

UParticleSystem* UEquipManager::GetBulletEffect()
{
	const FName& FoundItem = GetEquipItem(EEquipSlotType::Weapon);

	const FColorDataRow* ColorData = UMyLib::GetItemData(FoundItem).m_ColorHandle.GetRow<FColorDataRow>("");

	return ColorData->m_Bullet;
}

void UEquipManager::UpdateEquip()
{
	m_OnEquipChanged.Broadcast();
}
