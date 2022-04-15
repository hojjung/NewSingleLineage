// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestUseItem.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestUseItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_ItemExeManager->m_OnItemUse.AddUObject(this,&UQuestUseItem::OnItemUse);		
}

void UQuestUseItem::CompleteQuest()
{
	UMyGameInstance::Get->m_ItemExeManager->m_OnItemUse.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestUseItem::OnItemUse(const FName& itemID)
{
	if(m_Spec->m_QuestData->m_TargetName != itemID)
	{
		return;
	}

	IncreaseAmount();
}