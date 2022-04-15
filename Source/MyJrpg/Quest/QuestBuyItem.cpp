#include "QuestBuyItem.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UQuestBuyItem::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_ShopManager->m_OnItemBought.AddUObject(this,&UQuestBuyItem::OnItemBuy);		
}

void UQuestBuyItem::CompleteQuest()
{
	UMyGameInstance::Get->m_ShopManager->m_OnItemBought.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestBuyItem::OnItemBuy(const FName& itemID)
{
	if(m_Spec->m_QuestData->m_TargetName != itemID)
	{
		return;
	}

	IncreaseAmount();
}
