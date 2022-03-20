#include "WidgetQuestElement.h"
#include "MyJrpg/DataTables/QuestData.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Quest/QuestLogicBase.h"

void UWidgetQuestElement::Init(UQuestLogicBase* quest, bool isMain)
{
	Super::Init(quest, isMain);

	m_BtnComplete->OnClicked.AddDynamic(this,&UWidgetQuestElement::OnCompleteQuest);

	const FQuestSpec& QuestSpec = m_Quest->GetQuestSpec();

	m_PrizePanel->UpdatePrizeElement(*QuestSpec.m_QuestData);
}

void UWidgetQuestElement::OnClick()
{
	//Not Use
}

void UWidgetQuestElement::UpdateQuestElement()
{
	const FQuestSpec& QuestSpec = m_Quest->GetQuestSpec();

	FString QuestName = FString::Printf(TEXT("[%s] %s"),m_bIsMain ? TEXT("메인") : TEXT("서브"),*QuestSpec.m_QuestData->m_QuestName.ToString());

	m_TextQuestName->SetText(FText::FromString(QuestName));

	m_TextQuestDesc->SetText(QuestSpec.m_QuestData->m_QuestDesc);
	
	m_TextProgress->SetText(m_Quest->GetProgressText());
	
	m_BarProgress->SetPercent(m_Quest->GetProgress());
}

void UWidgetQuestElement::OnCompleteQuest()
{
	if(!m_Quest->CanCompleteQuest())
	{
		return;
	}
	
	UMyGameInstance::Get->m_QuestManager->CompleteQuest(m_Quest.Get());
}