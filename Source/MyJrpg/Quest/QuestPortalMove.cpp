// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestPortalMove.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestPortalMove::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_LevelMoveManager->m_OnLvelMoveComp.AddUObject(this,&UQuestPortalMove::OnPortalMove);		
}

void UQuestPortalMove::CompleteQuest()
{
	UMyGameInstance::Get->m_LevelMoveManager->m_OnLvelMoveComp.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestPortalMove::OnPortalMove(const FName& zoneName)
{
	if(m_Spec->m_QuestData->m_TargetName != zoneName)
	{
		return;
	}

	IncreaseAmount();
}