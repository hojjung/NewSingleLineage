#include "WidgetQuestHUD.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetQuestHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_QuestManager->m_OnQuestAdd.AddUObject(this, &UWidgetQuestHUD::AddChild);

	UMyGameInstance::Get->m_QuestManager->m_OnQuestRemove.AddUObject(this, &UWidgetQuestHUD::DeleteChild);

	CreateHUD();//깨면 삭제, 얻으면 추가, 바뀌면 업데이트
}

void UWidgetQuestHUD::CreateHUD()
{
	for(UQuestLogicBase* MQuest : UMyGameInstance::Get->m_QuestManager->GetMainQuests())
	{
		AddChild(MQuest,true);
	}

	for(UQuestLogicBase* SQuest : UMyGameInstance::Get->m_QuestManager->GetSubQuests())
	{
		AddChild(SQuest,false);
	}
}

void UWidgetQuestHUD::DeleteChild(UQuestLogicBase* q,bool isMain)
{
	UWidget* EraseWant = nullptr;
	
	for(UWidget* W : m_VertQuests->GetAllChildren())
	{
		UWidgetQuestHUDChild* QuestWidget = Cast<UWidgetQuestHUDChild>(W);

		if (QuestWidget->GetQuest() == q)
		{
			EraseWant = W;
			break;
		}
	}
	
	m_VertQuests->RemoveChild(EraseWant);
}

void UWidgetQuestHUD::AddChild(UQuestLogicBase* q,bool isMain)
{
	UWidgetQuestHUDChild* QuestChild = CreateWidget<UWidgetQuestHUDChild>(this, m_ClassHudChild);

	m_VertQuests->AddChildToVerticalBox(QuestChild);
	
	QuestChild->Init(q,isMain);
}
