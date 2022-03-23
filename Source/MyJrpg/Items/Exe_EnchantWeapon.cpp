#include "Exe_EnchantWeapon.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_EnchantWeapon::Use(const FName& item) const
{
	UMyGameInstance::Get->m_EnchantManager->SetMaterialEquip(item);
	UMyLib::GetCanvas()->OpenEnchant();
}
