#include "EnchantManager.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Exe_EnchantArmor.h"
#include "MyJrpg/Items/Exe_EnchantTrinket.h"
#include "MyJrpg/Items/Exe_EnchantWeapon.h"

UEnchantManager::UEnchantManager()
{
	Clear();
}

void UEnchantManager::SetTargetEquip(FItemSpec& target)
{
	m_CrntTarget = &target;

	m_OnEnchantChanged.Broadcast();
}

void UEnchantManager::SetMaterialEquip(FItemSpec& mat)
{
	m_CrntMat = &mat;

	if (m_CrntTarget && !IsAbleTarget(*m_CrntTarget))
	{
		m_CrntTarget = nullptr;
	}

	m_OnEnchantChanged.Broadcast();
}

bool UEnchantManager::IsAbleTarget(const FItemSpec& target)
{
	if (!m_CrntMat)
	{
		return nullptr;
	}
	const FItemDataRow& FoundTarget = UMyLib::GetItemData(target);
	
	const FItemDataRow& FoundMat = UMyLib::GetItemData(*m_CrntMat);
	
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

bool UEnchantManager::IsAbleMaterial(const FItemSpec& material)
{
	if (!m_CrntTarget)
	{
		return nullptr;
	}

	const FItemDataRow& FoundTarget = UMyLib::GetItemData(*m_CrntTarget);

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
	m_CrntMat = nullptr;

	m_CrntTarget = nullptr;
}
