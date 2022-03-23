#include "RewardManager.h"

#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/DropData.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Quest/Reward/RewardLogicBase.h"

void URewardManager::ReceiveQuestReward(const FQuestReward& qReward)
{
	if(qReward.m_nAmount<=0)
	{
		return;
	}

	if(qReward.m_Currency.RowName != NAME_None)
	{
		TSubclassOf<URewardLogicBase> LogicClass = qReward.m_Currency.GetRow<FCurrencyDataRow>("")->m_ClassReceiveLogic;

		if(!LogicClass->IsValidLowLevel())
		{
			return;
		}

		URewardLogicBase* LL =  NewObject<URewardLogicBase>(this,LogicClass);

		LL->ExecuteRewardReceive(qReward.m_nAmount);
		
	}
	else if(!qReward.m_Item.RowName.IsNone())
	{
		EItemType type = UMyLib::GetItemType(qReward.m_Item.RowName);

		if (type == EItemType::Equip)
		{
			FName HashID = UMyLib::GenerateEquipItemHashKey(qReward.m_Item.RowName,this);
			
			UMyGameInstance::Get->m_Inven->AddEquipItem(HashID);

			return;
		}
		
		UMyGameInstance::Get->m_Inven->AddItem(qReward.m_Item.RowName,qReward.m_nAmount);
	}
}

void URewardManager::RequestMonsterReward()
{
	const FDropDataRow* ZoneData = UMyGameInstance::Get->m_LevelMoveManager->GetDropData();

	if(!ZoneData)
	{
		return;
	}

	const TArray<FDropRewardItem>& AryDropDatas = ZoneData->m_AryDropItem;
	
	for(auto& DropItem : AryDropDatas)
	{
		int RandIndex = FMath::RandRange(0,DropItem.m_nExpectDropCount);

		if(RandIndex == 0)
		{
			int Amount = 1;
			
			UMyLib::GetPlayerInven()->AddItem(DropItem.m_Item.RowName,Amount);
		}
	}
}

bool URewardManager::RequestQuestReward(const TArray<FQuestReward>& aryQuest)
{
	bool Result = false;
	for(const FQuestReward& QQ : aryQuest)
	{
		if(QQ.m_Item.RowName != NAME_None)
		{
			int Amount  = QQ.m_nAmount;
			
			// if(EItemType::Equip == UMyLib::GetItemType(QQ.m_Item.RowName))
			// {
			// 	Result = UMyGameInstance::Get->m_Inven->CheckEmptySlot(Amount);
			//
			// 	if(!Result)
			// 	{
			// 		return false;
			// 	}
			// }
			// else
			// {
			// 	Result = UMyGameInstance::Get->m_Inven->CheckEmptyStack(QQ.m_Item.RowName,Amount);
			//
			// 	if(!Result)
			// 	{
			// 		Result = UMyGameInstance::Get->m_Inven->CheckEmptySlot(Amount);
			//
			// 		if(!Result)
			// 		{
			// 			return false;
			// 		}
			// 	}
			// }
		}
	}

	for(const FQuestReward& QQ : aryQuest)
	{
		ReceiveQuestReward(QQ);
	}

	return true;
}

void URewardManager::OnMonsterDead(const AMonsterPawn* monster)
{
	m_OnMonsterDead.Broadcast(monster->m_PawnName);

	UMyGameInstance::Get->m_CurrencyManager->AddGold(monster->GetRewardGold());

	UMyGameInstance::Get->m_PlayerStatManager->AddExp(monster->GetRewardExp());

	m_OnExpGold.Broadcast(monster->GetRewardExp(),monster->GetRewardGold());
	
	RequestMonsterReward();
}
