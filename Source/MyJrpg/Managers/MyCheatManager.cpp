// All right Reserve 2021 HereticByte


#include "MyCheatManager.h"

#include "MyGameInstance.h"
#include "RewardManager.h"
#include "Kismet/GameplayStatics.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UMyCheatManager::AddItemToPlayer(FName id, int amount)
{
	UMyLib::GetPlayerInven()->AddItem(FItemSpec(id,amount));
}

void UMyCheatManager::RemoveItemFromPlayer(FName id, int amount)
{
	//UMyLib::GetPlayer()->GetInven()->RemoveItem(id,amount);
}

void UMyCheatManager::RemoveItemFromFocusedTarget(FName id, int amount)
{
	if(!UMyLib::GetPlayer()->GetFocusedTarget())
	{
		return;
	}

	auto* FocusedPawn = Cast<ABaseUnitPawn>(UMyLib::GetPlayer()->GetFocusedTarget());

	if(!FocusedPawn)
	{
		return;
	}
	
	//FocusedPawn->GetInven()->RemoveItem(id,amount);
}

void UMyCheatManager::AddQuest(FName id)
{
	UMyGameInstance::Get->m_QuestManager->RegisterMainQuest(id);
}

void UMyCheatManager::TryCompleteQuest(FName id)
{
	
}