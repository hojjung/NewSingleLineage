// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogicBase.h"
#include "QuestSellItem.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestSellItem : public UQuestLogicBase
{
	GENERATED_BODY()

protected:
	virtual void RegisterQuest(FQuestSpec& spec) override;

	virtual void CompleteQuest() override;

	void OnItemSell(const FName& itemID);
};
