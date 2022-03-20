// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogicBase.h"
#include "QuestQuickSlotEquipItem.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UQuestQuickSlotEquipItem : public UQuestLogicBase
{
	GENERATED_BODY()

protected:
	virtual void RegisterQuest(FQuestSpec& spec) override;

	virtual void CompleteQuest() override;

	void OnEquipItem();
};
