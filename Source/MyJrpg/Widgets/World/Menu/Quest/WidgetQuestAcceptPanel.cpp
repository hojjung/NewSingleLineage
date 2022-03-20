// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetQuestAcceptPanel.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetQuestAcceptPanel::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();

	m_BtnAccept->OnClicked.AddDynamic(this,&UWidgetQuestAcceptPanel::OnAcceptQuest);
	
	m_BtnCancel->OnClicked.AddDynamic(this,&UWidgetQuestAcceptPanel::OnCancel);

	m_PrizePanel->Clear();
}

FReply UWidgetQuestAcceptPanel::NativeOnMouseButtonDown(const FGeometry& in_geometry,
	const FPointerEvent& in_mouse_event)
{
	UUserWidget::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

	return FReply::Handled();
}

FReply UWidgetQuestAcceptPanel::NativeOnTouchStarted(const FGeometry& in_geometry,
	const FPointerEvent& in_gesture_event)
{
	UUserWidget::NativeOnTouchStarted(in_geometry, in_gesture_event);

	return FReply::Handled();
}

void UWidgetQuestAcceptPanel::SetQuestAccept(const FName& qId)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_QuestID = qId;

	UpdateQuestElement();
}

void UWidgetQuestAcceptPanel::UpdateQuestElement()
{
	const FQuestDataRow* QuestDataFound = &UMyGameInstance::Get->m_QuestManager->GetSubQuestData(m_QuestID);

	//m_TextQuestName->SetText(QuestDataFound->m_QuestName);

	m_TextQuestDesc->SetText(QuestDataFound->m_QuestDesc);

	FString FormatStr = FString::Printf(TEXT("0/%d"),QuestDataFound->m_nMaxTargetAmount);

	//m_TextProgress->SetText(FText::FromString(FormatStr));

	m_PrizePanel->UpdatePrizeElement(*QuestDataFound);
}

void UWidgetQuestAcceptPanel::OnAcceptQuest()
{
	UMyGameInstance::Get->m_QuestManager->RegisterSubQuest(m_QuestID);

	OnCancel();
}

void UWidgetQuestAcceptPanel::OnCancel()
{
	SetVisibility(ESlateVisibility::Collapsed);

	m_PrizePanel->Clear();
}
