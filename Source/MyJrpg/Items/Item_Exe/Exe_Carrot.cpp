#include "Exe_Carrot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UExe_Carrot::Use(FItemSpec& item, UInventory* inven) const
{
	UMyGameInstance::Get->m_PlayerStatManager->AddHunger(3);
	
	float HealV = 3;

	UMyLib::GetPlayer()->TakeHeal(HealV);

	TryGetSeedItem();
}

void UExe_Carrot::TryGetSeedItem() const
{
	float Rand = FMath::RandRange(0.f,1.f);

	if(Rand < 0.4f)
	{
		return;
	}

	FItemSpec NewSeed(TEXT("Seed01"),1);
	
	UMyLib::AddItemAll(NewSeed,true);
}