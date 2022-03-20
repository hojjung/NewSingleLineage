// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLogicBase.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

bool UQuestLogicBase::CanCompleteQuest()
{
	return m_Spec->m_QuestData->m_nMaxTargetAmount<=m_Spec->m_nValue;
}

void UQuestLogicBase::CompleteQuest()
{
	PRINTF("Completed Quest!");
	//위젯 알림
	//보상 획득
	ReceiveRewards();
}

const FQuestSpec& UQuestLogicBase::GetQuestSpec() const
{
	return *m_Spec;
}

float UQuestLogicBase::GetProgress()
{
	return  (float)m_Spec->m_nValue / (float)m_Spec->m_QuestData->m_nMaxTargetAmount;
}

FText UQuestLogicBase::GetProgressText()
{
	FString FormatStr = FString::Printf(TEXT("(%d/%d)"),m_Spec->m_nValue,m_Spec->m_QuestData->m_nMaxTargetAmount);

	return FText::FromString(FormatStr);
}

void UQuestLogicBase::IncreaseAmount()
{
	if(CanCompleteQuest())
	{
		return;	
	}
	
	m_Spec->m_nValue++;

	PRINTF("Quest %d/%d",m_Spec->m_nValue,m_Spec->m_QuestData->m_nMaxTargetAmount);

	m_OnProgressChanged.Broadcast();
}

void UQuestLogicBase::ReceiveRewards()
{
	UMyGameInstance::Get->m_RewardManager->RequestQuestReward(m_Spec->m_QuestData->m_AryQuestReward);
}

void UQuestLogicBase::SetAmount(int v)
{
	m_Spec->m_nValue = v;

	m_OnProgressChanged.Broadcast();
}
