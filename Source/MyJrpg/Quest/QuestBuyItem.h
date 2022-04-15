#pragma once

#include "CoreMinimal.h"
#include "QuestLogicBase.h"
#include "QuestBuyItem.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestBuyItem : public UQuestLogicBase
{
	GENERATED_BODY()

protected:
	virtual void RegisterQuest(FQuestSpec& spec) override;

	virtual void CompleteQuest() override;

	void OnItemBuy(const FName& itemID);
};
