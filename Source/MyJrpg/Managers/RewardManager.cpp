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
			FName HashID = UMyLib::GenerateEquipItemHashKey(qReward.m_Item.RowName,this);
			
			UMyGameInstance::Get->m_Inven->AddEquipItem(HashID);

			return;
		}
		
		UMyGameInstance::Get->m_Inven->AddItem(qReward.m_Item.RowName,qReward.m_nAmount);
	}
}

void URewardManager::RequestMonsterReward()
{
	const FName& ZoneId = UMyGameInstance::Get->m_LevelMoveManager->GetCrntZoneID();

	const TArray<FDropRewardItem>* AryDropDatas = UMyGameInstance::Get->m_RewardManager->GetDropItems(ZoneId);

	if(AryDropDatas)
	{
		for(const auto& DropItem : *AryDropDatas)
		{
			int RandIndex = FMath::RandRange(0,DropItem.m_nExpectDropCount - 1);

			if(RandIndex == 0)
			{
				int Amount = 1;
			
				EItemType type = UMyLib::GetItemType(DropItem.m_Item.RowName);

				if (type == EItemType::Equip)
				{
					FName HashID = UMyLib::GenerateEquipItemHashKey(DropItem.m_Item.RowName,this);
			
					UMyGameInstance::Get->m_Inven->AddEquipItem(HashID);

					continue;;
				}
		
				UMyGameInstance::Get->m_Inven->AddItem(DropItem.m_Item.RowName,Amount);
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
			
			if(EItemType::Equip == UMyLib::GetItemType(QQ.m_Item.RowName))
			{
				if(UMyGameInstance::Get->m_Inven->GetRemainSlotCount() < Amount)
				{
					return false;
				}
			}
			else
			{
				if(UMyGameInstance::Get->m_Inven->GetAvailalbeStackCount(QQ.m_Item.RowName) < Amount)
				{
					return false;
				}
			}
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

void URewardManager::AddDropItemData(const FDropData& drop, const FName& itemID)
{
	if(m_MapDropItems.Contains(drop.m_ZoneUniqueID))
	{
		m_MapDropItems[drop.m_ZoneUniqueID].Emplace(FDropRewardItem(itemID, drop.m_nExpectDropCount));
		return;
	}

	FDropRewardItem DropItem (itemID, drop.m_nExpectDropCount);
	m_MapDropItems.Emplace(drop.m_ZoneUniqueID, TArray<FDropRewardItem>(&DropItem,1));
}

const TArray<FDropRewardItem>* URewardManager::GetDropItems(FName zoneID) const
{
	if(!m_MapDropItems.Contains(zoneID))
	{
		return nullptr;
	}
	return &m_MapDropItems[zoneID];
}
