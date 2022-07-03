#include "EE_Belt01.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UEE_Belt01::Equip()
{
	Super::Equip();
	
	UMyGameInstance::Get->m_EquipManager->EquipBelt(3);
}

void UEE_Belt01::UnEquip()
{
	Super::UnEquip();

	UMyGameInstance::Get->m_EquipManager->UnequipBelt();
}
