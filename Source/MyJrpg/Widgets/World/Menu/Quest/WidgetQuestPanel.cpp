#include "WidgetQuestPanel.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetQuestPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnMainQuest->OnClicked.AddDynamic(this,&UWidgetQuestPanel::ShowMainQuest);

	m_BtnSubQuest->OnClicked.AddDynamic(this,&UWidgetQuestPanel::ShowSubQuest);

	UMyGameInstance::Get->m_QuestManager->m_OnQuestAdd.AddUObject(this,&UWidgetQuestPanel::AddQuest);
	
	UMyGameInstance::Get->m_QuestManager->m_OnQuestRemove.AddUObject(this,&UWidgetQuestPanel::EraseQuest);

	ShowMainQuest();

	CreateChilds();
}

void UWidgetQuestPanel::CreateChilds()
{
	for(UQuestLogicBase* MQuest : UMyGameInstance::Get->m_QuestManager->GetMainQuests())
	{
		AddQuest(MQuest,true);
	}

	for(UQuestLogicBase* SQuest : UMyGameInstance::Get->m_QuestManager->GetSubQuests())
	{
		AddQuest(SQuest,false);
	}
}

void UWidgetQuestPanel::AddQuest(UQuestLogicBase* q, bool isMain)
{
	UWidgetQuestElement* QuestChild = CreateWidget<UWidgetQuestElement>(this, m_ClassQuestEle);

	if(isMain)
	{
		m_ScrollMainQuest->AddChild(QuestChild);
	}
	else
	{
		m_ScrollSubQuest->AddChild(QuestChild);
	}
	
	QuestChild->Init(q,isMain);
}

void UWidgetQuestPanel::EraseQuest(UQuestLogicBase* q, bool isMain)
{
	UWidget* EraseWant = nullptr;

	UScrollBox* TargetScrollParent =  isMain ? m_ScrollMainQuest : m_ScrollSubQuest;
	
	for(UWidget* W : TargetScrollParent->GetAllChildren())
	{
		UWidgetQuestElement* QuestWidget = Cast<UWidgetQuestElement>(W);

		if (QuestWidget->GetQuest() == q)
		{
			EraseWant = W;
			break;
		}
	}
	
	TargetScrollParent->RemoveChild(EraseWant);
}

void UWidgetQuestPanel::ShowMainQuest()
{
	m_ScrollMainQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ScrollSubQuest->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetQuestPanel::ShowSubQuest()
{
	m_ScrollMainQuest->SetVisibility(ESlateVisibility::Collapsed);

	m_ScrollSubQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
