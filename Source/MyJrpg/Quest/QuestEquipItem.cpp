// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestEquipItem.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestEquipItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this,&UQuestEquipItem::OnEquipItemChanged);		
}

void UQuestEquipItem::CompleteQuest()
{
	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestEquipItem::OnEquipItemChanged()
{
	for(const FItemSpec* AbSpec : UMyGameInstance::Get->m_EquipManager->GetEquipAry())
	{
		FText ItemName = UMyLib::GetItemData(AbSpec->m_ItemID).m_TextShowingName;
		
		if(m_Spec->m_QuestData->m_TargetName.EqualTo(ItemName))
		{
			IncreaseAmount();
			return;
		}
	}
}