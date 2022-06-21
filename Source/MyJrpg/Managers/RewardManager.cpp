#include "RewardManager.h"
#include "MyGameInstance.h"
#include "MyJrpg/MyLib.h"
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
			//FName HashID = UMyLib::GenerateEquipItemHashKey(qReward.m_Item.RowName,this);
			
			//UMyGameInstance::Get->m_Inven->AddItem(HashID);

			return;
		}
		
		//UMyGameInstance::Get->m_Inven->AddItem(qReward.m_Item.RowName,qReward.m_nAmount);
	}
}

void URewardManager::RequestMonsterReward(AMonsterPawn* mobPawn)
{
	const FNpcUnitEntityRow* NpcUnit = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(mobPawn->GetEntityID(), "");

	TArray<FDropRewardItem> DropReward = NpcUnit->m_AryDropItem;
	
	DropObtain(mobPawn->GetInven(), DropReward);
}

void URewardManager::DropObtain(UInventory* inven , const TArray<FDropRewardItem>& items)
{
	for(const auto& DropItem : items)
	{
		int RandIndex = FMath::RandRange(0,DropItem.m_nExpectDropCount - 1);

		if(RandIndex == 0)
		{
			int RandStackCount = FMath::RandRange(1,DropItem.m_nMaxStack);
				
			if (UMyLib::IsEquip(DropItem.m_Item.RowName))
			{
				inven->AddItem(FItemSpec(DropItem.m_Item.RowName, 0), true);
			}
			else
			{
				inven->AddItem(FItemSpec(DropItem.m_Item.RowName, RandStackCount));
			}
		}
	}
}

bool URewardManager::RequestQuestReward(const TArray<FQuestReward>& aryQuest)
{
	for(const FQuestReward& QQ : aryQuest)
	{
		if(QQ.m_Item.RowName != NAME_None)
		{
			int Amount  = QQ.m_nAmount;
			
			// if(EItemType::Equip == UMyLib::GetItemType(QQ.m_Item.RowName))
			// {
			// 	if(UMyGameInstance::Get->m_Inven->GetRemainSlotCount() < Amount)
			// 	{
			// 		return false;
			// 	}
			// }
			// else
			// {
			// 	if(UMyGameInstance::Get->m_Inven->GetAvailalbeStackCount(QQ.m_Item.RowName) < Amount)
			// 	{
			// 		return false;
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

void URewardManager::OnMonsterDead(AMonsterPawn* monster)
{
	m_OnMonsterDead.Broadcast(monster->GetEntityID());

	UMyGameInstance::Get->m_PlayerStatManager->AddExp(monster->GetRewardExp());

	m_OnExpGold.Broadcast(monster->GetRewardExp());
	
	RequestMonsterReward(monster);
}