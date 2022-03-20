#include "QuestKillMonsters.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UQuestKillMonsters::RegisterQuest(FQuestSpec& spec)
{
	m_Spec = &spec;

	m_QuestDeleHandle = UMyGameInstance::Get->m_RewardManager->m_OnMonsterDead.AddUObject(this,&UQuestKillMonsters::OnMonsterDead);		
}

void UQuestKillMonsters::CompleteQuest()
{
	UMyGameInstance::Get->m_RewardManager->m_OnMonsterDead.Remove(m_QuestDeleHandle);
	
	Super::CompleteQuest();
}

void UQuestKillMonsters::OnMonsterDead(const FText monsterName)
{
	if(!m_Spec->m_QuestData->m_TargetName.EqualTo(monsterName))
	{
		return;
	}

	IncreaseAmount();
}
