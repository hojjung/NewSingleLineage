// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestCraftItem.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestCraftItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_CraftManager->m_OnCraft.AddUObject(this,&UQuestCraftItem::OnItemCraft);		
}

void UQuestCraftItem::CompleteQuest()
{
	UMyGameInstance::Get->m_CraftManager->m_OnCraft.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestCraftItem::OnItemCraft(const FName& itemID)
{
	if(m_Spec->m_QuestData->m_TargetName != itemID)
	{
		return;
	}

	IncreaseAmount();
}
