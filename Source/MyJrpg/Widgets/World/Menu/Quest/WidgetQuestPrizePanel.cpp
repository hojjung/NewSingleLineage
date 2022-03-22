// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetQuestPrizePanel.h"

void UWidgetQuestPrizePanel::UpdatePrizeElement(const FQuestDataRow& questData)
{
	if(m_AryQuestEles.Num()>0)
	{
		for(UWidgetQuestPrizeElement* QuestPrize : m_AryQuestEles)
		{
			QuestPrize->UpdatePrize();
		}
		
		return;
	}
	
	m_AryQuestEles.Reset();

	for(const FQuestReward& QuestLogic : questData.m_AryQuestReward)
	{
		UWidgetQuestPrizeElement* QuestEle = CreateWidget<UWidgetQuestPrizeElement>(this, m_ClassQuestPrize);

		m_AryQuestEles.Add(QuestEle);

		m_WrapRewards->AddChild(QuestEle);

		QuestEle->SetPrize(QuestLogic);
	}
}

void UWidgetQuestPrizePanel::Clear()
{
	m_AryQuestEles.Reset();

	m_WrapRewards->ClearChildren();
}
