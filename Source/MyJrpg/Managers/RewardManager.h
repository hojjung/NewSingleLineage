// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/QuestData.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Items/Inventory.h"
#include "UObject/NoExportTypes.h"
#include "RewardManager.generated.h"

/**
 * 
 */

class AMonsterPawn;


UCLASS()
class MYJRPG_API URewardManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnExpGoldEarned, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDead,const FName&);
	
	FOnExpGoldEarned m_OnExpGold;

	FOnMonsterDead m_OnMonsterDead;

protected:
	TMap<FName,TArray<FDropRewardItem>> m_MapDropItems;

protected:
	void ReceiveQuestReward(const FQuestReward& qReward);

	void DropObtain(UInventory* inven , const TArray<FDropRewardItem>& items);
	
public:
	void RequestMonsterReward(AMonsterPawn* mobId);

	bool RequestQuestReward(const TArray<FQuestReward>& aryQuest);

	void OnMonsterDead(AMonsterPawn* monster);
	
	void AddDropItemData(const FDropData& drop, const FName& itemID);

	const TArray<FDropRewardItem>* GetDropItems(FName zoneID) const;
};
