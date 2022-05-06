#include "EquipManager.h"

#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"

void UEquipManager::Init()
{
	m_AryEqupSlots.Init(FItemSpec(),(int)EEquipSlotType::Length);

	m_Inven = UMyGameInstance::Get->m_Inven;
}

void UEquipManager::Equip(EEquipSlotType slotWant, int invenIndex)
{
	FItemSpec Item = m_Inven->GetItemRef(invenIndex);
	
	m_Inven->RemoveItem(invenIndex);
	
	Unequip(slotWant,&invenIndex);
	
	int SlotIndex = (int)slotWant;
	
	m_AryEqupSlots[SlotIndex] = Item;

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

bool UEquipManager::Unequip(EEquipSlotType slotWant, int * returnInvenIndex)
{
	PRINTF("Unequip01");
	int SlotIndex = (int)slotWant;

	FItemSpec Temp = m_AryEqupSlots[SlotIndex];
	if(Temp.m_ID.IsNone())
	{
		return true;
	}
	
	m_AryEqupSlots[SlotIndex] = FItemSpec();

	if(!returnInvenIndex)
	{
		m_Inven->AddItem(Temp);
	}
	else
	{
		m_Inven->AddItem(*returnInvenIndex,Temp);
	}
	
	SetIsRangeStance();
	
	m_OnEquipChanged.Broadcast();

	return true;
}

bool UEquipManager::IsItemEquipped(EEquipSlotType wantSlot)
{
	return !GetEquipItem((int)wantSlot).m_ID.IsNone();
}

const FItemSpec& UEquipManager::GetEquipItem(int slotIndex)
{
	return m_AryEqupSlots[slotIndex];
}

bool UEquipManager::IsRangeStance()
{
	return m_bIsRange;
}

void UEquipManager::SetIsRangeStance()
{
	const auto& FoundItem = GetEquipItem((int)EEquipSlotType::Weapon);

	if(FoundItem.m_ID.IsNone())
	{
		m_bIsRange = false;
		return ;
	}
	
	m_bIsRange = UMyLib::GetItemData(FoundItem.m_ID).m_bIsRange;
}

UParticleSystem* UEquipManager::GetBulletEffect()
{
	const auto& FoundItem = GetEquipItem((int)EEquipSlotType::Weapon);
	
	if(FoundItem.m_ID.IsNone())
	{
		m_bIsRange = false;
		return nullptr;
	}

	const FColorDataRow* ColorData = UMyLib::GetItemData(FoundItem.m_ID).m_ColorHandle.GetRow<FColorDataRow>("");

	return ColorData->m_Bullet;
}

void UEquipManager::UpdateEquip()
{
	m_OnEquipChanged.Broadcast();
}
