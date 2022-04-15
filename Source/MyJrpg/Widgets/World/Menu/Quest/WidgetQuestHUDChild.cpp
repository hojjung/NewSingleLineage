#include "WidgetQuestHUDChild.h"
#include "WidgetQuestHUD.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

FReply UWidgetQuestHUDChild::NativeOnMouseButtonDown(const FGeometry& in_geometry, const FPointerEvent& in_mouse_event)
{
	Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

	OnClick();

	return FReply::Handled();
}

FReply UWidgetQuestHUDChild::NativeOnTouchStarted(const FGeometry& in_geometry, const FPointerEvent& in_gesture_event)
{
	Super::NativeOnTouchStarted(in_geometry, in_gesture_event);

	OnClick();

	return FReply::Handled();
}

void UWidgetQuestHUDChild::Init(UQuestLogicBase* quest, bool isMain)
{
	SetQuest(quest,isMain);
	
	UMyGameInstance::Get->m_QuestManager->m_OnQuestChanged.AddUObject(this,&UWidgetQuestHUDChild::UpdateQuestElement);

	UpdateQuestElement();
}

void UWidgetQuestHUDChild::NativeDestruct()
{
	Super::NativeDestruct();

	if(m_Quest)
	{
		m_Quest->m_OnProgressChanged.Remove(m_DeleHandle);
	}
}

void UWidgetQuestHUDChild::OnClick()
{
	if(!m_Quest)
	{
		return;
	}

	UMyLib::GetCanvas()->OpenQuestPanel(m_bIsMain);
}

void UWidgetQuestHUDChild::SetQuest(UQuestLogicBase* quest, bool isMain)
{
	if (m_Quest == quest)
	{
		UpdateQuestElement();
		
		return;
	}
	m_bIsMain = isMain;
	
	if(m_Quest)
	{
		m_Quest->m_OnProgressChanged.Remove(m_DeleHandle);
	}
	
	m_Quest = quest;

	m_DeleHandle = m_Quest->m_OnProgressChanged.AddUObject(this,&UWidgetQuestHUDChild::UpdateQuestElement);

	UpdateQuestElement();
}

UQuestLogicBase* UWidgetQuestHUDChild::GetQuest()
{
	return m_Quest;
}

void UWidgetQuestHUDChild::UpdateQuestElement()
{
	const FQuestSpec& QuestSpec = m_Quest->GetQuestSpec();

	FString QuestName = FString::Printf(TEXT("[%s] %s"),m_bIsMain ? TEXT("메인") : TEXT("서브"),*QuestSpec.m_QuestData->m_QuestName.ToString());

	FString QuestDesc = FString::Printf(TEXT("%s %s"), *QuestSpec.m_QuestData->m_QuestDesc.ToString(), *m_Quest->GetProgressText().ToString());

	m_TextQuestName->SetText(FText::FromString(QuestName));

	m_TextQuestDesc->SetText(FText::FromString(QuestDesc));
}

