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
	int StLv = UMyLib::GetPlayerInven()->GetStLv(m_CrntTarget);

	UMyLib::GetPlayerInven()->SetStLv(m_CrntTarget, StLv + 1);

	UMyGameInstance::Get->m_CurrencyManager->SubGold(GetEnchantCost());
}

void UEnchantManager::EnchantFail()
{
	// if(UMyGameInstance::Get->m_EquipManager->IsItemEquipped(GetCrntTarget()))
	// {
	// 	UMyGameInstance::Get->m_EquipManager->Unequip(GetCrntTarget());
	// }
	//
	// m_InvenTarget.Get()->RemoveItem(GetCrntTarget(),m_CrntTarget);
	//
	// m_CrntTarget = NAME_None;
}

bool UEnchantManager::TryEnchant()
{
	float Percent = GetEnchantPercent();

	float Rand = FMath::RandRange(0.f,1.f);

	return Rand <= Percent;
}

int UEnchantManager::GetEnchantCost() const
{
	int Level = 1;
	
	if(m_CrntTarget > INDEX_NONE)
	{
		Level = m_nCrntLevel + 1;
	}
	return Level * 0;
}

int UEnchantManager::GetCrntLevel() const
{
	return m_nCrntLevel;
}

float UEnchantManager::GetEnchantPercent() const
{
	int Lv = m_nCrntLevel;
	
	if (Lv >= FGlobalVariable::ENCHANT_MAX)
	{
		return 0;
	}
	
	TSubclassOf<UItemExecuteBase> ClassExe = UMyLib::GetItemData(GetCrntMat()).m_ClassExeItem;

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

void UEnchantManager::SetTargetEquip(int target, UInventory* inven)
{
	m_CrntTarget = target;

	m_InvenTarget = inven;

	m_nCrntLevel = m_CrntTarget == INDEX_NONE ? 0 : m_InvenTarget.Get()->GetAryItems()[m_CrntTarget].m_nLvStack;

	m_OnEnchantChanged.Broadcast();
}

void UEnchantManager::SetMaterialEquip(int mat, UInventory* inven)
{
	m_CrntMat = mat;

	m_InvenMat = inven;

	if (m_CrntTarget != INDEX_NONE && !IsAbleTarget(GetCrntTarget()))
	{
		m_CrntTarget = NAME_None;
		m_nCrntLevel = 0;
	}

	m_OnEnchantChanged.Broadcast();
}

const FName& UEnchantManager::GetCrntTarget() const
{
	return UMyLib::GetPlayerInven()->GetAryItems()[m_CrntTarget].m_ID;;
}

const FName& UEnchantManager::GetCrntMat() const
{
	return UMyLib::GetPlayerInven()->GetAryItems()[m_CrntMat].m_ID;;
}

bool UEnchantManager::IsAbleTarget(const FName& target)
{
	if (m_CrntMat == INDEX_NONE)
	{
		return true;
	}
	const FItemDataRow& FoundTarget = UMyLib::GetItemData(target);
	
	const FItemDataRow& FoundMat = UMyLib::GetItemData(GetCrntMat());
	
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

bool UEnchantManager::IsAbleMaterial(const FName& material)
{
	if (m_CrntTarget == INDEX_NONE)
	{
		return false;
	}

	const FItemDataRow& FoundTarget = UMyLib::GetItemData(GetCrntTarget());

	const FItemDataRow& FoundMat = UMyLib::GetItemData(material);
	
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
	m_nCrntLevel = 0;
	
	m_CrntMat = INDEX_NONE;

	m_CrntTarget = INDEX_NONE;

	m_InvenMat = nullptr;

	m_InvenTarget = nullptr;
}

void UEnchantManager::DoEnchant()
{
	FName MatID = GetCrntMat();
	
	if (TryEnchant())
	{
		EnchantSuccess(false);
	}
	else
	{
		EnchantFail();
	}
	
	m_InvenMat.Get()->RemoveItem(GetCrntMat(), 1);

	if(!m_InvenMat.Get()->FindItem(MatID))
	{
		m_CrntMat = INDEX_NONE;
	}

	m_nCrntLevel = m_CrntTarget == INDEX_NONE ? 0 : m_InvenTarget.Get()->GetAryItems()[m_CrntTarget].m_nLvStack;

	m_OnEnchantChanged.Broadcast();
}

bool UEnchantManager::IsEnchantAvailable() const
{
	if (m_CrntMat == INDEX_NONE || m_CrntTarget == INDEX_NONE)
	{
		return false;
	}

	if (!UMyGameInstance::Get->m_CurrencyManager->CheckGoldEnough(GetEnchantCost()))
	{
		return false;
	}

	if (m_nCrntLevel >= FGlobalVariable::ENCHANT_MAX)
	{
		return false;
	}
	
	return true;
}
