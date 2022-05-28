#include "EnchantManager.h"

#include "EquipManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Item_Exe/Exe_EnchantArmor.h"
#include "MyJrpg/Items/Item_Exe/Exe_EnchantTrinket.h"
#include "MyJrpg/Items/Item_Exe/Exe_EnchantWeapon.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

UEnchantManager::UEnchantManager()
{
	m_AryWeaponPer[0] = 1.f;
	m_AryWeaponPer[1] = 1.f;
	m_AryWeaponPer[2] = 1.f;
	m_AryWeaponPer[3] = 1.f;
	m_AryWeaponPer[4] = 1.f;
	m_AryWeaponPer[5] = 1.f;
	m_AryWeaponPer[6] = 0.5f;
	m_AryWeaponPer[7] = 0.5f;
	m_AryWeaponPer[8] = 0.4f;
	m_AryWeaponPer[9] = 0.4f;
	m_AryWeaponPer[10] = 0.3f;
	m_AryWeaponPer[11] = 0.3f;
	m_AryWeaponPer[12] = 0.2f;
	m_AryWeaponPer[13] = 0.2f;
	m_AryWeaponPer[14] = 0.15f;
	m_AryWeaponPer[15] = 0.15f;
	m_AryWeaponPer[16] = 0.1f;
	m_AryWeaponPer[17] = 0.1f;
	m_AryWeaponPer[18] = 0.1f;
	m_AryWeaponPer[19] = 0.1f;

	m_AryArmorPer[0] = 1.f;
	m_AryArmorPer[1] = 1.f;
	m_AryArmorPer[2] = 1.f;
	m_AryArmorPer[3] = 1.f;
	m_AryArmorPer[4] = 1.f;
	m_AryArmorPer[5] = 0.6f;
	m_AryArmorPer[6] = 0.6f;
	m_AryArmorPer[7] = 0.5f;
	m_AryArmorPer[8] = 0.5f;
	m_AryArmorPer[9] = 0.4f;
	m_AryArmorPer[10] = 0.4f;
	m_AryArmorPer[11] = 0.3f;
	m_AryArmorPer[12] = 0.3f;
	m_AryArmorPer[13] = 0.2f;
	m_AryArmorPer[14] = 0.2f;
	m_AryArmorPer[15] = 0.15f;
	m_AryArmorPer[16] = 0.15f;
	m_AryArmorPer[17] = 0.1f;
	m_AryArmorPer[18] = 0.1f;
	m_AryArmorPer[19] = 0.1f;
	
	m_AryTrinketPer[0] = 1.f;
	m_AryTrinketPer[1] = 0.6f;
	m_AryTrinketPer[2] = 0.6f;
	m_AryTrinketPer[3] = 0.5f;
	m_AryTrinketPer[4] = 0.5f;
	m_AryTrinketPer[5] = 0.4f;
	m_AryTrinketPer[6] = 0.4f;
	m_AryTrinketPer[7] = 0.3f;
	m_AryTrinketPer[8] = 0.3f;
	m_AryTrinketPer[9] = 0.2f;
	m_AryTrinketPer[10] = 0.2f;
	m_AryTrinketPer[11] = 0.15f;
	m_AryTrinketPer[12] = 0.15f;
	m_AryTrinketPer[13] = 0.15f;
	m_AryTrinketPer[14] = 0.15f;
	m_AryTrinketPer[15] = 0.1f;
	m_AryTrinketPer[16] = 0.1f;
	m_AryTrinketPer[17] = 0.1f;
	m_AryTrinketPer[18] = 0.1f;
	m_AryTrinketPer[19] = 0.1f;
	
	Clear();
}

void UEnchantManager::EnchantSuccess(bool isSpecial)
{
	int StLv = m_TargetItem->m_nLvStack;

	m_TargetItem->m_nLvStack = StLv + 1;
	
	UMyLib::GetPlayerInven()->UpdateInventory();
}

void UEnchantManager::EnchantFail()
{
	m_InvenTarget.Get()->RemoveItem(*m_TargetItem);
	
	m_TargetItem = nullptr;
}

bool UEnchantManager::TryEnchant()
{
	float Percent = GetEnchantPercent();

	float Rand = FMath::RandRange(0.f,1.f);

	return Rand <= Percent;
}

int UEnchantManager::GetCrntLevel() const
{
	return  m_TargetItem == nullptr ? 0 : m_TargetItem->m_nLvStack;
}

float UEnchantManager::GetEnchantPercent() const
{
	int Lv = GetCrntLevel();
	
	if (Lv >= FGlobalVariable::ENCHANT_MAX)
	{
		return 0;
	}
	
	TSubclassOf<UItemExecuteBase> ClassExe = UMyLib::GetItemData(GetTargetMat()->m_ID).m_ClassExeItem;

	if (ClassExe == UExe_EnchantWeapon::StaticClass())
	{
		return m_AryWeaponPer[Lv];
	}
	else if (ClassExe == UExe_EnchantArmor::StaticClass())
	{
		return m_AryArmorPer[Lv];
	}
	else if (ClassExe == UExe_EnchantTrinket::StaticClass())
	{
		return m_AryTrinketPer[Lv];
	}
	return 0;
}

void UEnchantManager::SetTargetEquip(FItemSpec& target, UInventory* inven)
{
	m_TargetItem = &target;

	m_InvenTarget = inven;

	if (GetTargetMat() && !IsAbleMaterial(*GetTargetMat()))
	{
		m_TargetMat = nullptr;
	}

	m_OnEnchantChanged.Broadcast();
}

void UEnchantManager::SetMaterialEquip(FItemSpec& mat, UInventory* inven)
{
	m_TargetMat = &mat;

	m_InvenMat = inven;

	if (GetTargetItem() && !IsAbleTarget(*GetTargetItem()))
	{
		m_TargetItem = nullptr;
	}

	m_OnEnchantChanged.Broadcast();
}

const FItemSpec* UEnchantManager::GetTargetItem() const
{
	return m_TargetItem;
}

const FItemSpec* UEnchantManager::GetTargetMat() const
{
	return m_TargetMat;
}

bool UEnchantManager::IsAbleTarget(const FItemSpec& target) const
{
	const FItemDataRow& FoundTarget = UMyLib::GetItemData(target.m_ID);

	if(!UMyLib::IsEquip(FoundTarget))
	{
		return false;
	}
	
	if (!GetTargetMat())
	{
		return true;
	}
	
	const FItemDataRow& FoundMat = UMyLib::GetItemData(GetTargetMat()->m_ID);
	
	if (FoundTarget.m_ItemType == EEquipSlotType::Weapon)
	{
		return UExe_EnchantWeapon::StaticClass() == FoundMat.m_ClassExeItem;
	}
	else if ((int)EEquipSlotType::Head <= (int)FoundTarget.m_ItemType && (int)FoundTarget.m_ItemType <= (int)EEquipSlotType::Leg)
	{
		return UExe_EnchantArmor::StaticClass() == FoundMat.m_ClassExeItem;
	}
	else if ((int)EEquipSlotType::Ring <= (int)FoundTarget.m_ItemType && (int)FoundTarget.m_ItemType <= (int)EEquipSlotType::Belt)
	{
		return UExe_EnchantTrinket::StaticClass() == FoundMat.m_ClassExeItem;
	}
	
	return false;
}

bool UEnchantManager::IsAbleMaterial(const FItemSpec& material) const
{
	if (!GetTargetItem())
	{
		return true;
	}

	const FItemDataRow& FoundTarget = UMyLib::GetItemData(GetTargetItem()->m_ID);

	const FItemDataRow& FoundMat = UMyLib::GetItemData(material.m_ID);
	
	if (FoundTarget.m_ItemType == EEquipSlotType::Weapon)
	{
		return UExe_EnchantWeapon::StaticClass() == FoundMat.m_ClassExeItem;
	}
	else if ((int)EEquipSlotType::Head <= (int)FoundTarget.m_ItemType && (int)FoundTarget.m_ItemType <= (int)EEquipSlotType::Leg)
	{
		return UExe_EnchantArmor::StaticClass() == FoundMat.m_ClassExeItem;
	}
	else if ((int)EEquipSlotType::Ring <= (int)FoundTarget.m_ItemType && (int)FoundTarget.m_ItemType <= (int)EEquipSlotType::Belt)
	{
		return UExe_EnchantTrinket::StaticClass() == FoundMat.m_ClassExeItem;
	}
	
	return false;
}

void UEnchantManager::Clear()
{
	m_TargetItem = nullptr;

	m_TargetMat = nullptr;

	m_InvenMat = nullptr;

	m_InvenTarget = nullptr;
}

void UEnchantManager::DoEnchant()
{
	if (TryEnchant())
	{
		EnchantSuccess(false);
	}
	else
	{
		EnchantFail();
	}
	
	m_InvenMat.Get()->RemoveItem(*GetTargetMat(),1);

	if(m_TargetMat->m_nLvStack < 1)
	{
		m_TargetMat = nullptr;
	}

	m_OnEnchantChanged.Broadcast();

	m_InvenMat->UpdateInventory();

	m_InvenTarget->UpdateInventory();
}

bool UEnchantManager::IsEnchantAvailable() const
{
	if (!GetTargetItem())
	{
		return false;
	}

	if(!GetTargetMat() || !IsAbleMaterial(*GetTargetMat()))
	{
		return false;
	}
	if (GetCrntLevel() >= FGlobalVariable::ENCHANT_MAX)
	{
		return false;
	}
	
	return true;
}
