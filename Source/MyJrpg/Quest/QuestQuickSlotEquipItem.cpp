// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestQuickSlotEquipItem.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestQuickSlotEquipItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	//m_QuestDeleHandle = UMyGameInstance::Get->m_QuickSlotManager->m_OnQuickSlotChanged.AddUObject(this,&UQuestQuickSlotEquipItem::OnEquipItem);
}

void UQuestQuickSlotEquipItem::CompleteQuest()
{
	//UMyGameInstance::Get->m_QuickSlotManager->m_OnQuickSlotChanged.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestQuickSlotEquipItem::OnEquipItem()
{
	//if(!UMyGameInstance::Get->m_QuickSlotManager->CheckHasElement(m_Spec->m_QuestData->m_TargetName))
	{
		return;
	}

	IncreaseAmount();
}
