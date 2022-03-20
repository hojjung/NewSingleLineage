// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogicBase.h"
#include "QuestKillMonsters.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestKillMonsters : public UQuestLogicBase
{
	GENERATED_BODY()

protected:
	virtual void RegisterQuest(FQuestSpec& spec) override;

	virtual void CompleteQuest() override;

	void OnMonsterDead(const FText monsterName);
};
