#include "QuestWithdrawItem.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestWithdrawItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_AryStorage[0]->m_OnNewItemRemoved.AddUObject(this,&UQuestWithdrawItem::OnItemRemovedInStorage);		
}

void UQuestWithdrawItem::CompleteQuest()
{
	UMyGameInstance::Get->m_AryStorage[0]->m_OnNewItemRemoved.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestWithdrawItem::OnItemRemovedInStorage(const FName& itemName)
{
	if(m_Spec->m_QuestData->m_TargetName != itemName)
	{
		return;
	}

	IncreaseAmount();
}
