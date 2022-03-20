// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogicBase.h"
#include "QuestCraftItem.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestCraftItem : public UQuestLogicBase
{
	GENERATED_BODY()

protected:
	virtual void RegisterQuest(FQuestSpec& spec) override;

	virtual void CompleteQuest() override;

	void OnItemCraft(const FText monsterName);
};
