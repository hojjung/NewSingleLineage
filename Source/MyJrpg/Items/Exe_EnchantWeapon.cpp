#include "Exe_EnchantWeapon.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_EnchantWeapon::Use(const FName& item,UInventory* inven) const
{
	UMyLib::GetCanvas()->OpenEnchant();
	//UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(item,inven);
}
