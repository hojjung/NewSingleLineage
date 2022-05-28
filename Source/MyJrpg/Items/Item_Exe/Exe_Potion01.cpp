#include "Exe_Potion01.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_Potion01::Use(FItemSpec& item,UInventory* inven) const
{
	float HealV = 40;

	UMyLib::GetPlayer()->TakeHeal(HealV);
}
