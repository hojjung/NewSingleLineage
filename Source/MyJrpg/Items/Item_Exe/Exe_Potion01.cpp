#include "Exe_Potion01.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_Potion01::Use() const
{
	float HealV = 40;

	UMyLib::GetPlayer()->TakeHeal(HealV);
}
