#include "QuestSellItem.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestSellItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_ShopManager->m_OnItemSell.AddUObject(this,&UQuestSellItem::OnItemSell);		
}

void UQuestSellItem::CompleteQuest()
{
	UMyGameInstance::Get->m_ShopManager->m_OnItemSell.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestSellItem::OnItemSell(const FName& itemID)
{
	if(m_Spec->m_QuestData->m_TargetName != itemID)
	{
		return;
	}

	IncreaseAmount();
}
