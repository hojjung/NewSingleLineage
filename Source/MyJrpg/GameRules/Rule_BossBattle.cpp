#include "Rule_BossBattle.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void URule_BossBattle::OnMonsterCreate(AMonsterPawn* monster_pawn)
{
	if(!monster_pawn->IsBoss())
	{
		return;
	}
	//create bossui
}

void URule_BossBattle::OnMonsterDead(AMonsterPawn* monster_pawn)
{
	UMyGameInstance::Get->m_RewardManager->OnMonsterDead(monster_pawn);
	
	if(!monster_pawn->IsBoss())
	{
		return;
	}
	//End game, player should not take damage
	//show ui
}
